#include <iostream>
#include <memory>
#include <string>

#include "XP_Certi.h"

using std::string;
using std::auto_ptr;

extern	FILE		*fLog;																				//Log File
extern	Federe	*FedereXPlane ;																	//Ambassador
extern	enum		{ leX,leY,leZ,Phi,The,Psi} dataPlane;										//TODO : datas
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Publish Values
void Federe::registreObjAndpublishVal()
{
try
{
auto_ptr<RTI::AttributeHandleSet> attributes(RTI::AttributeHandleSetFactory::create(nbData));

IDClass= getObjectClassHandle("Aircraft");

ID_Att[leX]= getAttributeHandle("Att_Lat",IDClass); attributes->add(ID_Att[leX]);	//TODO : datas
ID_Att[leY]= getAttributeHandle("Att_Lon",IDClass); attributes->add(ID_Att[leY]);
ID_Att[leZ]= getAttributeHandle("Att_Hte",IDClass); attributes->add(ID_Att[leZ]);
ID_Att[Phi]= getAttributeHandle("Att_Phi",IDClass); attributes->add(ID_Att[Phi]);
ID_Att[The]= getAttributeHandle("Att_The",IDClass); attributes->add(ID_Att[The]);
ID_Att[Psi]= getAttributeHandle("Att_Psi",IDClass); attributes->add(ID_Att[Psi]);

fprintf(fLog,"Publish attributes of class %d ",IDClass); fflush(fLog);
publishObjectClass(IDClass, *attributes);

fprintf(fLog,"Register object %s ",federationName); fflush(fLog);
HObject= registerObjectInstance(IDClass,federationName);
fprintf(fLog,"Made\n"); fflush(fLog);
}

catch ( RTI::Exception &e) 
	{
	fprintf(fLog,"!!Error : Publish Value : %s\n",e._reason) ;
	exit(-1);
}	}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Update Values
void upDateValues(float *fData, short nbE)
{
RTI::AttributeHandleValuePairSet *attributeSet= RTI::AttributeSetFactory::create(nbE);

if (FedereXPlane->pass == false)
	{//Save datas on the first call
	FedereXPlane->pass= true;
	
	FedereXPlane->setMemData(fData[leX],leX);
	FedereXPlane->setMemData(fData[leY],leY);
	FedereXPlane->setMemData(fData[leZ],leZ);
	
	fData[0]= fData[1]= fData[2]= 0.0f;
	}
else
	{//Adjust Value of xyz
	fData[leX]-= FedereXPlane->getMemData(leX);
	fData[leY]-= FedereXPlane->getMemData(leY);
	fData[leZ]-= FedereXPlane->getMemData(leZ);
	}

for (short i= leX; i < nbE; i++)
	attributeSet->add(FedereXPlane->ID_Att[i], (char*)&fData[i],sizeof(fData[i]));  

FedereXPlane->updateAttributeValues(FedereXPlane->HObject,*attributeSet,FedereXPlane->getFederateName());
}