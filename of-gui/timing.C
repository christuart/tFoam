#include "timing.H"

uint64 GetTimeMs64()
{
#ifdef _WIN32
 /* Windows */
 FILETIME ft;
 LARGE_INTEGER li;

 /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
  * to a LARGE_INTEGER structure. */
 GetSystemTimeAsFileTime(&ft);
 li.LowPart = ft.dwLowDateTime;
 li.HighPart = ft.dwHighDateTime;

 uint64 ret = li.QuadPart;
 ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
 ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

 return ret;
#else
 /* Linux */
 struct timeval tv;

 gettimeofday(&tv, NULL);

 uint64 ret = tv.tv_usec;
 /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
 ret /= 1000;

 /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
 ret += (tv.tv_sec * 1000);

 return ret;
#endif
}

double average_historic_time(double _history_times[], int _history_count) {
	if (_history_count < 1) {
		std::cerr << "Asked for time based on empty history." << std::endl;
		throw;
	}
	size_t i;
	double a = 0.0;
	for (i=0; i < _history_count; ++i) {
		a += _history_times[i];
	}
	return a/_history_count;
}

std::string format_time(double time) {

	int reductions = 0;
	// time is submitted in seconds
	if (time > 60.0) {
		time /= 60.0; // to minutes
		++reductions;
		if (time > 60.0) {
			time /= 60.0; // to hours
			++reductions;
			if (time > 24.0) {
				time /= 24.0; // to days
				++reductions;
				if (time > 30.437) { // to months
					time /= 30.436666666666667;
					++reductions;
					if (time > 12.0) { // to years
						time /= 12.0;
						++reductions;
					}
				}
			}
		}
	}
	std::ostringstream oss;
	char temp[20];
	
	sprintf(temp, "%9.2f", time);
	oss << std::string(temp);
	switch (reductions) {
	case 0:	oss << " sec.  "; break;
	case 1:	oss << " min.  "; break;
	case 2:	oss << " hours "; break;
	case 3:	oss << " days  "; break;
	case 4:	oss << " months"; break;
	case 5:	oss << " years "; break;
	}
	return oss.str();
	     
}
