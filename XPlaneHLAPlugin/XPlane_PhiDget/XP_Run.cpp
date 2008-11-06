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
extern	sPHI		*PHI;					//PhiDJet			

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
void	upDateValues(char *name, int lng, float value)
{
short		i,j,id;

for (j= 0; j < gNbClasse; j++)
	for (id= CLA[j].ptAttribute, i= 0; i < CLA[j].nbAttribute; i++,id++) if (!strncmp(name,HLA[id].aName,lng))	 
		{		
		RTI::AttributeHandleValuePairSet *attributeSet= RTI::AttributeSetFactory::create(1);
		
		attributeSet->add(HLA[id].aHandle, (char*)&value,sizeof(float));
		theFederate->updateAttributeValues(CLA[j].cHandle,*attributeSet,theFederate->getFederateName());
		break;
		}
}