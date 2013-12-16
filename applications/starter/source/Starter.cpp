#include <iostream>

#include <common/include/Logger.h>
#include <database/include/Database.h>

using namespace mkay;
using namespace std;

int main(void)
{
  Logger::init_defaults();
  
  loginf << "starting up ..." << endl;
  
  Database db("test.sqlite");
}
