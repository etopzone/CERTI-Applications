/**
 * HLA Class core	
 */
typedef struct structCLA
{//Class Part----------------------------------------------
char							cName[NBCAR+1];	/*!< Class  Name */
RTI::ObjectHandle			cHandle;				/*!< Object Handle */
int							nbAttribute;		/*!< attributes */
int							ptAttribute;		/*!< Index to Attributes table */
} sCLA;

/**
 * HLA Attribute core	
 */
typedef struct structHLA
{//Attribute Part------------------------------------------
char							aName[NBCAR+1];	/*!< Name	*/
char							format[5];			/*!< Format */
RTI::AttributeHandle		aHandle;				/*!< Handle */
} sHLA;