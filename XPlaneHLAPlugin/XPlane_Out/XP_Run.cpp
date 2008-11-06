#include <iostream>
#include <memory>
#include <string>

#include "XP_Certi.h"

using std::string;
using std::auto_ptr;

extern	FILE		*fLog;				//Log File
extern	Federe	*theFederate ;	//Ambassador

extern	int		gNbClasse;			//# classes 
extern	int		gNbAttrib;			//# classes 
extern	sCLA		*CLA;					//HHLA Class core			
extern	sHLA		*HLA;					//HLA Attribute core			
extern	sXPL		*XPL;					//Xplane core			

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Publish Values
void Federe::registreObjAndpublishVal()
{
short		i,j,id;
for (j= 0; j < gNbClasse; j++)
	{
	RTI::ObjectClassHandle	IDClass;
	auto_ptr<RTI::AttributeHandleSet> attributes(RTI::AttributeHandleSetFactory::create(CLA[j].nbAttribute));
	
	IDClass= getObjectClassHandle(CLA[j].cName);
	
	for (id= CLA[j].ptAttribute, i= 0; i < CLA[j].nbAttribute; i++,id++)
		{
		HLA[id].aHandle= getAttributeHandle(HLA[id].aName,IDClass);
		attributes->add(HLA[id].aHandle);
		}
		
	//Record the current class			
	fprintf(fLog,"Publish attributes of class %d ",IDClass); fflush(fLog);
	try { publishObjectClass(IDClass, *attributes);	}
	catch ( RTI::Exception &e) 
		{ fprintf(fLog,"!!Error :publishObjectClass : %s\n",e._reason) ; exit(-1);}

	fprintf(fLog,"Register object %s ",federationName); fflush(fLog);
	try { CLA[j].cHandle= registerObjectInstance(IDClass);}
	catch ( RTI::Exception &e) 
		{ fprintf(fLog,"!!Error : registerObjectInstance : %s\n",e._reason) ; exit(-1);}
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Update Values
void upDateValues()
{
short		i,j,id;


for (j= 0; j < gNbClasse; j++)
	{
	RTI::AttributeHandleValuePairSet *attributeSet= RTI::AttributeSetFactory::create(CLA[j].nbAttribute);

	for (id= CLA[j].ptAttribute, i= 0; i < CLA[j].nbAttribute; i++,id++)
		{		
		switch (XPL[id].XPType)
			{
			case xplmType_Int:			attributeSet->add(HLA[id].aHandle, (char*)&XPL[id].XPValueI,sizeof(int));		break;
			case xplmType_Float:			attributeSet->add(HLA[id].aHandle, (char*)&XPL[id].XPValueF,sizeof(float));		break;
			case xplmType_Double:		attributeSet->add(HLA[id].aHandle, (char*)&XPL[id].XPValueD,sizeof(double));	break;
			
			case xplmType_IntArray:		attributeSet->add(HLA[id].aHandle, (char*)XPL[id].XPBlockI,sizeof(int)	* XPL[id].XPLengthB); break;
			case xplmType_FloatArray:	attributeSet->add(HLA[id].aHandle, (char*)XPL[id].XPBlockF,sizeof(float)	* XPL[id].XPLengthB); break;		
			case xplmType_Data:			attributeSet->add(HLA[id].aHandle, (char*)XPL[id].XPBlockB,sizeof(BYTE)	* XPL[id].XPLengthB); break;
		}	}
	theFederate->updateAttributeValues(CLA[j].cHandle,*attributeSet,theFederate->getFederateName());
	}  
}