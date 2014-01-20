#include <common/include/Time_measurement.h>

using namespace std;

namespace mkay 
{
  void Time_measurement::take_time()
  {
    auto now = chrono::high_resolution_clock::now();
    m_round_time = chrono::duration_cast<chrono::microseconds>(now - m_last);
    m_last = now;
  }
}