#include <iostream>
#include <sstream>

#include <unistd.h>
#include <signal.h>

#include <common/include/Mkay_exception.h>
#include <common/include/Logger.h>
#include <module/include/Configuration_exception.h>
#include <erosion/include/Module_erosion.h>

#include <generated/include/version_gen_fwd.hpp>

#include <GL/gl.h>
#include <SDL2/SDL.h>

#include <boost/program_options.hpp>

using namespace mkay;
using namespace std;

namespace po = boost::program_options;

static volatile bool g_exit_flag = false;
static std::string g_parameter_description{""};
static std::string g_program_name{""};

void signal_handler(int signum) 
{
  std::stringstream os;
  os << "received signal: ";
  switch (signum) 
  {
    case SIGHUP:
      os << "SIGHUP";
      break;
    case SIGINT:
      os << "SIGINT";
      g_exit_flag = true;
      break;
    case SIGTERM:
      os << "SIGTERM";
      g_exit_flag = true;
      break;
    case SIGQUIT:
      os << "SIGQUIT";
      g_exit_flag = true;
      break;
    case SIGUSR1:
      os << "SIGUSR1";
      break;
    case SIGPIPE:
      os << "SIGPIPE";
      break;
    case SIGALRM:
      os << "SIGALRM";
      break;
    case SIGCHLD:
      os << "SIGCHLD";
      break;
    case SIGXFSZ:
      os << "SIGCHLD";
      break;
    default:
      break;
  }
  logdeb << os.str() << std::endl;
}

/// Module Creation Factory
Module_ptr_t create_module(std::string const & i_module_name)
{
  Module_ptr_t module;
  if ( i_module_name == Module_erosion::get_module_type() )
  {
    module = Module_ptr_t( new Module_erosion() );
  }
  return module;
}

void build_description(po::options_description & o_description, Module_ptr_t i_program)
{ 
  logdeb << "adding generic options" << endl;
  po::options_description generic("Generic Options");
  generic.add_options()
    ("help", "produce help message")
    ("version,v", "output version information")
    ("module", po::value<string>(), "instantiate module name")
  ;
  o_description.add(generic);
  
  if ( i_program )
  {
    logdeb << "adding module options" << endl;
    po::options_description module("Module Options");
    i_program->get_options_description(module);
    o_description.add(module);
  } 
  
  std::stringstream ostr;
  ostr << o_description;
  g_parameter_description = ostr.str();
}

void usage()
{
  std::cerr << endl
            << "Usage: " << g_program_name << " [options]" << endl
            << g_parameter_description << endl;
}

void parse_arguments(int argc, char **argv, po::variables_map & i_parameters, Module_ptr_t i_program = Module_ptr_t{})
{
  try
  {
    logdeb << "parsing arguments" << std::endl;
    po::options_description description;
    build_description(description, i_program);
   ;
    if ( !i_program )
    {
      po::command_line_parser clp(argc, argv);
      po::store( clp.options(description).allow_unregistered().run(), i_parameters);
    }
    else
    {
      po::store( po::parse_command_line(argc, argv, description), i_parameters);
    }
    
    po::notify(i_parameters);
  }
  catch( po::unknown_option const & ex )
  {
  }
  catch( po::invalid_command_line_syntax const & ex )
  {
    logerr << ex.what() << endl;
    usage();
    exit(3);
  }
  catch( po::required_option const & ex )
  {
    logerr << ex.what() << endl;
    usage();
    exit(4);
  }
}

int main(int argc, char ** argv)
{ 
  g_program_name = string{argv[0]};
  
  signal(SIGHUP,  signal_handler);
  signal(SIGINT,  signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGUSR1, signal_handler);
  signal(SIGPIPE, signal_handler);
  signal(SIGALRM, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGCHLD, signal_handler);
  signal(SIGXFSZ, signal_handler);
  
  Logger::init_defaults();
  Logger::set_verbosity(Log_level::Debug);
  
  try
  {
    Module_ptr_t program;
    po::variables_map vm;
    string module_name;
    
    // parse default arguments
    parse_arguments(argc, argv, vm);
    
    if(vm.count("version"))
    {
      loginf << "Version information following" << endl
             << "Compiled on: " << COMPILATION_DATE << endl
             << Version_info::get_complete_version_info() << endl;
      exit(0);
    }
    
    // get module name if the user entered one
    if(vm.count("module"))
    {
      module_name = vm["module"].as<string>();
    }
    
    // exit if module name is empty or help was called (and module name is empty)
    if ( module_name.empty() || (vm.count("help") && module_name.empty()) )
    {
      usage();
      exit(1);
    }
    
    // create module name
    program = create_module(module_name);
    if ( !program )
    {
      logerr << "could not create module: " << module_name << endl;
      exit(10);
    }
    
    // build description together with program parameters
    parse_arguments(argc, argv, vm, program);
    
    if ( vm.count("help") )
    {
      usage();
      exit(2);
    }

    try
    {
      loginf << "configuring module" << std::endl;
      program->configure(vm);
    }
    catch(Configuration_exception const & config_ex)
    {
      logerr << boost::diagnostic_information(config_ex);
      usage();
      exit(5);
    }
      
    loginf << "starting execution" << std::endl;
    while ( false == program->requests_exit() && false == g_exit_flag )
    {
      program->step();
      usleep(1000);
    }
    
    loginf << "commencing shutdown" << std::endl;
    program->shutdown();
  }
  catch (Mkay_exception const & ex)
  {
    logerr << boost::diagnostic_information(ex);
  }
  
  loginf << "end of line" << std::endl;
}
