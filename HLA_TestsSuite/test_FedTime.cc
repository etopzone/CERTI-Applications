#include <iostream>
#include <RTI.hh>
#include <fedtime.hh>

std::string printable(RTI::FedTime& aFedTime) {
	char* myPrintable;
	std::string retval;
	
	int i = aFedTime.getPrintableLength();
	myPrintable = new char[i+1];	
	aFedTime.getPrintableString(myPrintable);	
	retval = std::string(myPrintable);
	delete[] myPrintable;	
	return retval;
}

int main(void)
{
  int retcode = 0;
  std::cout << "RTI::FedTime test <BEGIN>" << std::endl;
  RTI::FedTime* providedTime = RTI::FedTimeFactory::makeZero();
  RTI::FedTime* currentTime  = new RTIfedTime(16384.0);
  RTI::FedTime* lookahead    = new RTIfedTime(0.10000000000000000555);  
  
  std::cout.precision(20);
  std::cout << "            currentTime = " << printable(*currentTime) <<std::endl;
  std::cout << "              lookahead = " << printable(*lookahead) <<std::endl;
  *providedTime = *currentTime;
  *providedTime += *lookahead;
  std::cout << "currentTime + lookahead = " << printable(*providedTime) << std::endl;
  std::cout << "RTI::FedTime test <BEGIN>" << std::endl;
  return retcode;
}
