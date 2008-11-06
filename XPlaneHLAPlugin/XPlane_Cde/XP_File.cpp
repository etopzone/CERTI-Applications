#include <iostream>
#include <memory>
#include <string>
#include <map>

#include "XP_Trace.h"

using std::string;

extern	FILE		*fLog;				/*!< Log File				*/
extern	Federe	*theFederate ;		/*!< Ambassador			*/

extern	int		gNbClasse;			/*!< # classes 			*/
extern	int		gNbAttrib;			/*!< # classes 			*/
extern	sCLA		*CLA;					/*!< /HLA Class core		*/		
extern	sHLA		*HLA;					/*!< HLA Attribute core	*/		
extern	sPHI		*PHI;					/*!< PhiDJet				*/		

void razLF_RC(char *nameF)
{//----------------------------------------------------------------------------
short	lng;

do
	{
	lng= strlen(nameF) - 1;			//Delete "line feed" if exist
	if ((nameF[lng] == 10) || (nameF[lng] == 13)) nameF[lng]= 0;
	} while ((nameF[lng] == 10) || (nameF[lng] == 13));
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Read Descriptor
char Federe::readFileInit()
{
FILE	*theFile= fopen("XPlaneCde.txt","r"); if (theFile == NULL)	return(0x01);

fscanf(theFile,"%s\n",federationName);	//Federation name :-)
fgets(federationDesc,259,theFile);		//Fedfile (\installed\share\federations\xxxxx.fed)
fgets(dictionaryName,259,theFile);		//Dictionary XPlane-FOM name :-)
fscanf(theFile,"%s\n",IP_RTIg);			//Server RTIG IP address
fclose (theFile);

razLF_RC(federationDesc);					//Delete "line feed / Carriage Return" if exist
razLF_RC(dictionaryName);					//Delete "line feed / Carriage Return" if exist

//Read Dictionary file---------------------------------------------------------
char		theS[200];

//First step-----------------------------------------------------------------------------
theFile= fopen(dictionaryName,"r"); if (theFile == NULL)			return(0x02);

gNbClasse= gNbAttrib= 0; 
char	refClass[NBCAR+1]; refClass[0]= 0;
while (fgets(theS,200,theFile)) 
	{
	switch (theS[0])
		{
		case '#'://Comments
			break;
		case '§'://New class
			gNbClasse++; break;
		default://New attributs
			gNbAttrib++; break;		
	}	}

if (ferror (theFile))														return(0x04);
fclose (theFile);

CLA= (sCLA*)malloc(gNbClasse * sizeof(sCLA));			//Allocation memory
HLA= (sHLA*)malloc(gNbAttrib * sizeof(sHLA));
PHI= (sPHI*)malloc(gNbAttrib * sizeof(sPHI));

//Second step----------------------------------------------------------------------------
theFile= fopen(dictionaryName,"r"); if (theFile == NULL)			return(0x08);

gNbClasse= gNbAttrib= 0; 
while (fgets(theS,200,theFile))
	{
	razLF_RC(theS);	
	
	switch (theS[0])
		{
		case 0:
		case '#'://Comments
																					break;

		case '§'://New class
			strcpy(CLA[gNbClasse].cName,&theS[1]);						//Class Name
			CLA[gNbClasse].ptAttribute= gNbAttrib;						//Index pointing first element of class
			CLA[gNbClasse].nbAttribute= 0;								//Figure of attributes
			gNbClasse++;														break;

		default://New attributs
			string	line(theS);												//Get pattern
			short	finC= line.find('!');									//End of Format
			short	finX= line.find('=');									//XPlane Attribut
			
			if (finC == string::npos)										return(0x10);
			if (finX == string::npos)										return(0x20);

			string	fmt= line.substr(0,4);								//Format
			string	fom= line.substr(finC+1,finX-finC-1);			//HLA Attribute Name
			string	xpl= line.substr(finX+1);							//XPlane Attribute Name
			
			strcpy(HLA[gNbAttrib].aName,	fom.c_str());
			strcpy(HLA[gNbAttrib].format,			fmt.c_str());

			strcpy(PHI[gNbAttrib].Name,			xpl.c_str());
			strcpy(PHI[gNbAttrib].format,			fmt.c_str());
			
			CLA[gNbClasse - 1].nbAttribute++;
			gNbAttrib++;														break;		
	}	}
	
if (ferror (theFile))														return(0x40);
fclose (theFile);

//qsort(HLA,nbHlaXp,sizeof(sHLA_XP),sortClass);
return(0);
}


