#include <iostream>
#include <chrono>
#include <thread>
#include <future>
using namespace std;

bool TABULAR = false; // Print out results on single line in tabular form

#define THREAD_MAX 4     // The max number of threads to run on
#ifdef THREAD_MAX // Declare and init nt, the num of threads to use
  const long nt = (THREAD_MAX<thread::hardware_concurrency()) ? \
THREAD_MAX : thread::hardware_concurrency();
#else
  const long nt = thread::hardware_concurrency();
#endif

void printRate(long nums, double seconds); // Print the formatted rate

volatile long curr = 0;

// Checks for 3 and no more 6s in a row somewhere
bool checkSix(long x){
  if( x<666 )
    return false;
  long z = x / 1000;
  long y = x - 1000*z;
  if( y<600 || y >= 700 )
    return checkSix(z);
  if( y<660 || y >= 670 )
    return checkSix(x/100);
  if( y!=666 )
    return checkSix(x/10);
  x = z;
  if( x%10 != 6 )
    return true;
  while( (x/=10)%10 == 6 );
  return checkSix(x);  
}

// Recursive version of finding any 3 consecutive 6s
bool checkH(long x){
  if( x<666 )
    return false;
  long z = x / 1000;
  long y = x - 1000*z;
  if( y<600 || y>=700 )
    return checkH(z);
  if( y<660 || y>=670 )
    return checkH(x/100);
  if( y!=666 )
    return checkH(x/10);
  return true;
}
bool check(long x){
  long z = x / 1000;
  long y = x - 1000*z;
  if( y<600 || y>=700 )
    return checkH(z);
  if( y<660 || y>=670 )
    return checkH(x/100);
  if( y!=666 )
    return checkH(x/10);
  return true;
}


struct Result {
  long c0 = 0;
  long c1 = 0;
};

// Single threaded counter
Result count(long start, long stop){
  Result r;
  long c0 = 0;
  long c1 = 0;
  for(long i=start; i<stop; ++i){
    if(check(i)){
      ++c0;
      if(checkSix(i))
        ++c1;
    }
  }
  r.c0 = c0;
  r.c1 = c1;
  return r;
}

// Mutlithreaded counter
Result countMT(long start, long stop){
  long ps = (stop - start)/nt;
  long i;
  Result r;
  std::future<struct Result> rs[nt - 1];

  for(i=0; i<nt-1; ++i)
    rs[i] = async(launch::async, count, i*ps+start, (1+i)*ps+start);
  
  r = count(i*ps+start, stop);

  for(i=0; i<nt-1; ++i){
    Result tr = rs[i].get();
    r.c0 += tr.c0;
    r.c1 += tr.c1;
  }
  return r;
}
int main(int argc, char *argv[]) {
  long max = 10;
  long start;
  int n = 3;
  if(argc>1){
    if(argc>2) TABULAR=true;
    n = atoi(argv[1]);
    int t = n;
    if(t < 1 || t > 19){
      cout << "NUM DIGITS MUST BE IN [1,19]" << endl;
      return 2;
    } 
    while(--t>0) max*=10;
    start = max/10;
  } else {
    cout << "SPECIFY NUMBER OF DIGITS" << endl;
    return 1;
  }
  long total = max - start;
  Result r;
  
  if(!TABULAR) cout << "Checking numbers " << start << " to " << max << " on " \
    << nt << " threads." << endl;
  
  auto t1 = chrono::high_resolution_clock::now();
  if(THREAD_MAX > 1)
    r = countMT(start, max);
  else 
    r = count(start, max);
  auto t2 = chrono::high_resolution_clock::now();

  if (TABULAR){
    cout << n << '\t' << r.c0 << '\t' << r.c1 << endl;
  } else {
    cout << n << '\t' << r.c0 << '\t' << r.c1 << endl;
    long duration = (long)chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();
    double seconds = duration/1000000.0;
    cout << "Got count in " << duration << " us (" << seconds << " seconds)" << endl;
    printRate(total, seconds);
    cout << "\nOut of all (" << total << ") " <<  n << " digit numbers:\n\t" << \
r.c0 << "\tcontain '666'\n\t" << r.c1 << "\tcontain exactly three 6s in a row" << endl;
  }
  return 0;
}

void printRate(long nums, double seconds){
  long rate = nums / seconds;
  int parts[5]= {};
  long r;
  int i;
  for(i=0; rate > 0 && i<5; ++i){
    r = rate/1000;
    parts[i] = rate - 1000*r;
    rate = r;
  }
  cout << "Rate = " << parts[--i];
  for(--i; i>=0; --i){
    int p = parts[i];
    cout << ',';
    if (p<100){
      if(p<10) cout << "00";
      else cout << '0';
    }
    cout <<  p;
  }
  cout << " numbers/second" << endl;
}
