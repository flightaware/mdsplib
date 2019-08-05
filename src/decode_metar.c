/*
METAR Decoder Software Package Library: Parses Aviation Routine Weather Reports
Copyright (C) 2003  Eric McCarthy

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <string.h>
#include "metar_structs.h"     /* standard header file */

float fracPart( char * );
void decode_metar_remark( char **, Decoded_METAR * );
void print_decoded_metar( Decoded_METAR * );

/********************************************************************/
/*                                                                  */
/*  Title:         SaveTokenString                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          14 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      SaveTokenString tokenizes the input character    */
/*                 string based upon the delimeter set supplied     */
/*                 by the calling routine.  The elements tokenized  */
/*                 from the input character string are saved in an  */
/*                 array of pointers to characters.  The address of */
/*                 this array is the output from this function.     */
/*                                                                  */
/*  Input:         string - a pointer to a character string.        */
/*                                                                  */
/*                 delimeters - a pointer to a string of 1 or more  */
/*                              characters that are used for token- */
/*                              izing the input character string.   */
/*                                                                  */
/*  Output:        token  - the address of a pointer to an array of */
/*                          pointers to character strings.  The     */
/*                          array of pointers are the addresses of  */
/*                          the character strings that are token-   */
/*                          ized from the input character string.   */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static char **SaveTokenString ( char *string , char *delimeters )
{
   int NDEX;
   static char *token[ MAXTOKENS ], *TOKEN;
 
   /* TOKENIZE THE INPUT CHARACTER STRING    */
   /* AND SAVE THE TOKENS TO THE token ARRAY */
 
   NDEX = 0;
   TOKEN = strtok(string, delimeters);
 
   if( TOKEN == NULL )
      return NULL;
 
   token[NDEX] = (char *) malloc(sizeof(char)*(strlen(TOKEN)+1));
   strcpy( token[ NDEX ], TOKEN );
 
 
   while ( token[NDEX] != NULL )
   {
      NDEX++;
      TOKEN = strtok(NULL, delimeters);
 
      if( TOKEN != NULL )
      {
         token[NDEX] = (char *)
                              malloc(sizeof(char)*(strlen(TOKEN)+1));
         strcpy( token[NDEX], TOKEN );
      }
      else
         token[ NDEX ] = TOKEN;
 
   }
 
 
   return token;
 
}

/********************************************************************/
/*                                                                  */
/*  Title:         freeTokens                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          14 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      freeTokens frees the storage allocated for the   */
/*                 character strings stored in the token array.     */
/*                                                                  */
/*  Input:         token  - the address of a pointer to an array    */
/*                          of string tokens.                       */
/*                                                                  */
/*                                                                  */
/*  Output:        None.                                            */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static void freeTokens( char **token )
{
   int NDEX;
 
   NDEX = 0;
   while( *(token+NDEX) != NULL )
   {
      free( *(token+NDEX) );
      NDEX++;
   }
   return;
}

/********************************************************************/
/*                                                                  */
/*  Title:         init_metar_decoder                               */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  init_metar_decoder initializes every member of the   */
/*             structure addressed by the pointer Mptr.             */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         Mptr - ptr to a decoded_METAR structure.         */
/*                                                                  */
/*  Output:        NONE                                             */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

static void init_metar_decoder( Decoded_METAR *Mptr )
{
   int i, j;
 
   memset(Mptr->TS_LOC, '\0', sizeof(Mptr->TS_LOC));
   memset(Mptr->TS_MOVMNT,'\0', sizeof(Mptr->TS_MOVMNT));
 
 
   memset(Mptr->TornadicType, '\0', sizeof(Mptr->TornadicType));
   memset(Mptr->TornadicLOC, '\0', sizeof(Mptr->TornadicLOC));
   memset(Mptr->TornadicDIR, '\0', sizeof(Mptr->TornadicDIR));
   memset(Mptr->TornadicMovDir, '\0', sizeof(Mptr->TornadicMovDir));
   Mptr->BTornadicHour = MAXINT;
   Mptr->BTornadicMinute = MAXINT;
   Mptr->ETornadicHour = MAXINT;
   Mptr->ETornadicMinute = MAXINT;
   Mptr->TornadicDistance = MAXINT;
 
   memset( Mptr->autoIndicator,'\0', sizeof(Mptr->autoIndicator));
 
   Mptr->RVRNO = FALSE;
   Mptr->GR = FALSE;
   Mptr->GR_Size = (float) MAXINT;
 
   Mptr->CHINO = FALSE;
   memset(Mptr->CHINO_LOC, '\0', sizeof(Mptr->CHINO_LOC));
 
   Mptr->VISNO = FALSE;
   memset(Mptr->VISNO_LOC, '\0', sizeof(Mptr->VISNO_LOC));
 
   Mptr->PNO = FALSE;
   Mptr->PWINO = FALSE;
   Mptr->FZRANO  = FALSE;
   Mptr->TSNO   = FALSE;
   Mptr->DollarSign  = FALSE;
   Mptr->hourlyPrecip = (float) MAXINT;
 
   Mptr->ObscurAloftHgt = MAXINT;
   memset(Mptr->ObscurAloft, '\0', sizeof(Mptr->ObscurAloft));
   memset(Mptr->ObscurAloftSkyCond, '\0', sizeof(Mptr->ObscurAloftSkyCond));
 
   memset(Mptr->VrbSkyBelow, '\0', sizeof(Mptr->VrbSkyBelow));
   memset(Mptr->VrbSkyAbove, '\0', sizeof(Mptr->VrbSkyAbove));
   Mptr->VrbSkyLayerHgt = MAXINT;
 
   Mptr->SectorVsby = (float) MAXINT;
   memset( Mptr->SectorVsby_Dir, '\0', sizeof(Mptr->SectorVsby_Dir));
 
   memset(Mptr->codeName, '\0', sizeof(Mptr->codeName));
   memset(Mptr->stnid, '\0', sizeof(Mptr->stnid));
   Mptr->ob_hour   = MAXINT;
   Mptr->ob_minute = MAXINT;
   Mptr->ob_date   = MAXINT;
 
   memset(Mptr->synoptic_cloud_type, '\0', sizeof(Mptr->synoptic_cloud_type));
 
   Mptr->CloudLow    = '\0';
   Mptr->CloudMedium = '\0';
   Mptr->CloudHigh   = '\0';
 
   memset(Mptr->snow_depth_group, '\0', sizeof(Mptr->snow_depth_group));
   Mptr->snow_depth = MAXINT;
 
   Mptr->Temp_2_tenths    = (float) MAXINT;
   Mptr->DP_Temp_2_tenths = (float) MAXINT;
 
   Mptr->OCNL_LTG      = FALSE;
   Mptr->FRQ_LTG       = FALSE;
   Mptr->CNS_LTG       = FALSE;
   Mptr->CG_LTG        = FALSE;
   Mptr->IC_LTG        = FALSE;
   Mptr->CC_LTG        = FALSE;
   Mptr->CA_LTG        = FALSE;
   Mptr->AP_LTG        = FALSE;
   Mptr->OVHD_LTG      = FALSE;
   Mptr->DSNT_LTG      = FALSE;
   Mptr->VcyStn_LTG    = FALSE;
   Mptr->LightningVCTS = FALSE;
   Mptr->LightningTS   = FALSE;
   Mptr->visibilityIsUpperBound = FALSE;
 
   memset( Mptr->LTG_DIR, '\0', sizeof(Mptr->LTG_DIR));
 
 
   for( i = 0; i < 3; i++)
   {
      memset(Mptr->ReWx[i].Recent_weather, '\0', sizeof(Mptr->ReWx[i].Recent_weather));
 
      Mptr->ReWx[i].Bhh = MAXINT;
      Mptr->ReWx[i].Bmm = MAXINT;
 
      Mptr->ReWx[i].Ehh = MAXINT;
      Mptr->ReWx[i].Emm = MAXINT;
 
   }
 
   Mptr->NIL_rpt = FALSE;
   Mptr->AUTO = FALSE;
   Mptr->COR  = FALSE;
 
   Mptr->winData.windDir = MAXINT;
   Mptr->winData.windSpeed = MAXINT;
   Mptr->winData.windGust = MAXINT;
   Mptr->winData.windVRB  = FALSE;
   memset(Mptr->winData.windUnits, '\0', sizeof(Mptr->winData.windUnits));
 
   Mptr->minWnDir = MAXINT;
   Mptr->maxWnDir = MAXINT;
 
   memset(Mptr->horiz_vsby, '\0', sizeof(Mptr->horiz_vsby));
   memset(Mptr->dir_min_horiz_vsby, '\0', sizeof(Mptr->dir_min_horiz_vsby));
 
   Mptr->prevail_vsbySM = (float) MAXINT;
   Mptr->prevail_vsbyM  = (float) MAXINT;
   Mptr->prevail_vsbyKM = (float) MAXINT;
 
   memset(Mptr->vsby_Dir, '\0', sizeof(Mptr->vsby_Dir));
 
   Mptr->CAVOK = FALSE;
 
   for ( i = 0; i < MAX_RUNWAYS; i++ )
   {
      memset(Mptr->RRVR[ i ].runway_designator,
              '\0', sizeof(Mptr->RRVR[i].runway_designator));
 
      Mptr->RRVR[ i ].visRange = MAXINT;
 
      Mptr->RRVR[ i ].vrbl_visRange = FALSE;
      Mptr->RRVR[ i ].below_min_RVR = FALSE;
      Mptr->RRVR[ i ].above_max_RVR = FALSE;
 
 
      Mptr->RRVR[ i ].Max_visRange = MAXINT;
      Mptr->RRVR[ i ].Min_visRange = MAXINT;

      Mptr->RRVR[ i ].distance_unit = DIST_FEET;
   }
 
   Mptr->DVR.visRange = MAXINT;
   Mptr->DVR.vrbl_visRange = FALSE;
   Mptr->DVR.below_min_DVR = FALSE;
   Mptr->DVR.above_max_DVR = FALSE;
   Mptr->DVR.Max_visRange = MAXINT;
   Mptr->DVR.Min_visRange = MAXINT;
 
   for ( i = 0; i < MAXWXSYMBOLS; i++ )
   {
      for( j = 0; j < 8; j++ )
         Mptr->WxObstruct[i][j] = '\0';
   }
 
   // partial obscuration
 
    for (i = 0; i < MAX_PARTIAL_OBSCURATIONS; i++) {
       memset( Mptr->PartialObscurationAmt[i], '\0', sizeof(Mptr->PartialObscurationAmt[i]));
       memset( Mptr->PartialObscurationPhenom[i], '\0', sizeof(Mptr->PartialObscurationPhenom[0]));
    }
 
 
   // cloud type, cloud level, and significant clouds
 
 
   for ( i = 0; i < MAX_CLOUD_GROUPS; i++ )
   {
      memset(Mptr->cloudGroup[ i ].cloud_type,
              '\0', sizeof(Mptr->cloudGroup[i].cloud_type));
 
      memset(Mptr->cloudGroup[ i ].cloud_hgt_char,
              '\0', sizeof(Mptr->cloudGroup[i].cloud_hgt_char));
 
      Mptr->cloudGroup[ i ].cloud_hgt_meters = MAXINT;
 
      memset(Mptr->cloudGroup[ i ].other_cld_phenom,
              '\0', sizeof(Mptr->cloudGroup[i].other_cld_phenom));
   }
 
   Mptr->VertVsby = MAXINT;
 
   Mptr->temp = MAXINT;
   Mptr->dew_pt_temp = MAXINT;
   Mptr->QFE = MAXINT;
 
   Mptr->SLPNO = FALSE;
   Mptr->SLP = (float) MAXINT;
 
   Mptr->A_altstng = FALSE;
   Mptr->inches_altstng = (double) MAXINT;
 
   Mptr->Q_altstng = FALSE;
   Mptr->hectoPasc_altstng = MAXINT;
 
   Mptr->char_prestndcy = MAXINT;
   Mptr->prestndcy = (float) MAXINT;
 
   Mptr->precip_amt = (float) MAXINT;
 
   Mptr->precip_24_amt = (float) MAXINT;
   Mptr->maxtemp       = (float) MAXINT;
   Mptr->mintemp       = (float) MAXINT;
   Mptr->max24temp     = (float) MAXINT;
   Mptr->min24temp     = (float) MAXINT;
 
   Mptr->VIRGA         = FALSE;
   memset( Mptr->VIRGA_DIR, '\0', sizeof(Mptr->VIRGA_DIR));
 
   Mptr->VOLCASH       = FALSE;
 
   Mptr->minCeiling    = MAXINT;
   Mptr->maxCeiling    = MAXINT;
 
   Mptr->CIG_2ndSite_Meters = MAXINT;
   memset(Mptr->CIG_2ndSite_LOC, '\0', sizeof(Mptr->CIG_2ndSite_LOC));
 
   Mptr->minVsby = (float) MAXINT;
   Mptr->maxVsby = (float) MAXINT;
   Mptr->VSBY_2ndSite = (float) MAXINT;
   memset(Mptr->VSBY_2ndSite_LOC, '\0', sizeof(Mptr->VSBY_2ndSite_LOC));
 
   for( i = 0; i < 6; i++ )
      memset (Mptr->SfcObscuration[i], '\0', sizeof(Mptr->SfcObscuration[i]));
 
   Mptr->Num8thsSkyObscured = MAXINT;
 
   Mptr->Indeterminant3_6HrPrecip = FALSE;
   Mptr->Indeterminant_24HrPrecip = FALSE;
   Mptr->CIGNO = FALSE;
   Mptr->Ceiling = MAXINT;
   Mptr->Estimated_Ceiling = MAXINT;
 
   Mptr->NOSPECI = FALSE;
   Mptr->LAST    = FALSE;
 
   Mptr->SNINCR = MAXINT;
   Mptr->SNINCR_TotalDepth = MAXINT;
 
   Mptr->WaterEquivSnow = (float) MAXINT;
 
   Mptr->SunshineDur = MAXINT;
   Mptr->SunSensorOut = FALSE;
 
 
   Mptr->WshfTime_hour = MAXINT;
   Mptr->WshfTime_minute = MAXINT;
   Mptr->Wshft_FROPA     = FALSE;
   Mptr->min_vrbl_wind_dir = MAXINT;
   Mptr->max_vrbl_wind_dir = MAXINT;
 
   Mptr->PRESRR        = FALSE;
   Mptr->PRESFR        = FALSE;
 
   Mptr->TWR_VSBY = (float) MAXINT;
   Mptr->SFC_VSBY = (float) MAXINT;
 
   Mptr->PKWND_dir = MAXINT;
   Mptr->PKWND_speed = MAXINT;
   Mptr->PKWND_hour = MAXINT;
   Mptr->PKWND_minute = MAXINT;
 
   return;
 
}

/********************************************************************/
/*                                                                  */
/*  Title:         ResetMETARGroup                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  ResetMETARGroup returns a METAR_obGroup enumerated   */
/*             variable that indicates which METAR reporting group  */
/*             might next appear in the METAR report and should be  */
/*             considered for decoding.                             */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         StartGroup - a METAR_obGroup variable that       */
/*                              indicates where or on what group    */
/*                              METAR Decoding began.               */
/*                                                                  */
/*                 SaveStartGroup - a METAR_obGroup variable that   */
/*                                  indicates the reporting group   */
/*                                  in the METAR report that was    */
/*                                  successfully decoded.           */
/*                                                                  */
/*  Output:        A METAR_obGroup variable that indicates which    */
/*                 reporting group in the METAR report should next  */
/*                 be considered for decoding                       */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

static int ResetMETARGroup( int StartGroup,
                            int SaveStartGroup )
{
 
   enum METAR_obGroup { codename, stnid, NIL1, COR1, obDateTime, NIL2,
                        AUTO, COR, windData, MinMaxWinDir,
                        CAVOK, visibility,
                        RVR, presentWX, skyCond, tempGroup,
                        altimStng, NotIDed = 99};
 
   if( StartGroup == NotIDed && SaveStartGroup == NotIDed )
      return NotIDed;
   else if( StartGroup == NotIDed && SaveStartGroup != NotIDed &&
            SaveStartGroup != altimStng )
      return (++SaveStartGroup);
   else
      return (++SaveStartGroup);
 
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         CodedHgt2Meters                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  CodedHgt2Meters converts a coded cloud height into   */
/*             meters.                                              */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         token - a pointer to a METAR report group.       */
/*                 Mptr - a pointer to a decoded_METAR structure.   */
/*                                                                  */
/*  Output:        Cloud height in meters                           */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static int CodedHgt2Meters( char *token, Decoded_METAR *Mptr )
{
   int hgt;
   static int maxhgt = 30000;
 
 
   if( (hgt = atoi(token)) == 999 )
      return maxhgt;
   else
      return (hgt*30);
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isPartialObscuration                             */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  isPartialObscuration determines whether or not the   */
/*             METAR report element that is passed to it is or is   */
/*             not a partial obscuration indicator for an amount of */
/*             obscuration.                                         */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         token - the address of a pointer to the group    */
/*                         in the METAR report that                 */
/*                         isPartialObscuration determines is       */
/*                         or is not a partial                      */
/*                         obscuration indicator.                   */
/*                                                                  */
/*                                                                  */
/*                 Mptr - a pointer to a decoded_METAR structure.   */
/*                                                                  */
/*  Output:        TRUE, if the group is a partial obscuration      */
/*                 indicator and FALSE, if it is not.               */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

static MDSP_BOOL isPartialObscuration( char **string, Decoded_METAR *Mptr,
                          int *NDEX )
{
 
   if( *string == NULL )
      return FALSE;
 
   if( strcmp( *string, "FEW///" ) == 0 ||
       strcmp( *string, "SCT///" ) == 0 ||
       strcmp( *string, "BKN///" ) == 0 ||
       strcmp( *string, "FEW000" ) == 0 ||
       strcmp( *string, "SCT000" ) == 0 ||
       strcmp( *string, "BKN000" ) == 0    ) {
      strcpy( Mptr->PartialObscurationAmt[0], *string );
      (*NDEX)++;
      string++;
 
      if( *string == NULL )
         return TRUE;
 
      if( strcmp( (*string+3), "///") ) {
          if( strcmp( *string, "FEW000" ) == 0 ||
              strcmp( *string, "SCT000" ) == 0 ||
              strcmp( *string, "BKN000" ) == 0    ) {
            strcpy( Mptr->PartialObscurationAmt[1], *string );
            (*NDEX)++;
         }
      }
      else {
         if( strcmp( *string, "FEW///" ) == 0 ||
             strcmp( *string, "SCT///" ) == 0 ||
             strcmp( *string, "BKN///" ) == 0 ) {
            strcpy( Mptr->PartialObscurationAmt[1], *string );
            (*NDEX)++;
         }
      }
      return TRUE;
   }
   else
      return FALSE;
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isCloudLayer                                     */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      isCloudLayer determines whether or not the       */
/*                 current group has a valid cloud layer            */
/*                 identifier.                                      */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         token - pointer to a METAR report group.         */
/*                                                                  */
/*  Output:        TRUE, if the report group is a valid cloud       */
/*                 layer indicator.                                 */
/*                                                                  */
/*                 FALSE, if the report group is not a valid cloud  */
/*                 layer indicator.                                 */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isCloudLayer( char *token )
{
   if( token == NULL )
      return FALSE;

   if (strlen(token) == 6 && charcmp(token,"'/''/''/''/''/''/'")) {
      return TRUE;
   }
 
   if( strlen(token) < 6 )
      return FALSE;
   else
      return ((strncmp(token,"OVC",3) == 0 ||
               strncmp(token,"SCT",3) == 0 ||
               strncmp(token,"FEW",3) == 0 ||
               strncmp(token,"BKN",3) == 0 ||
               (isdigit(*token) &&
                strncmp(token+1,"CU",2) == 0) ||
               (isdigit(*token) &&
                strncmp(token+1,"SC",2) == 0) ) &&
               nisdigit((token+3),3)) ? TRUE:FALSE;
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isCAVOK                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          09 May 1996                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      isCAVOK determines whether or not the current    */
/*                 group is a valid CAVOK indicator.                */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         token - pointer to a METAR report group.         */
/*                                                                  */
/*  Output:        TRUE, if the input group is a valid CAVOK        */
/*                 indicator.  FALSE, otherwise.                    */
/*                                                                  */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isCAVOK( char *token, Decoded_METAR *Mptr, int *NDEX )
{
 
   if( token == NULL )
      return FALSE;
 
   if( strcmp(token, "CAVOK") != 0 )
      return FALSE;
   else {
      (*NDEX)++;
      Mptr->CAVOK = TRUE;
      return TRUE;
   }
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         parseCloudData                                   */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static void parseCloudData( char *token, Decoded_METAR *Mptr, int next)
{
 
 
   if( token == NULL )
      return;

   if (strlen(token) == 6 && charcmp(token,"'/''/''/''/''/''/'")) {
      return;
   }
 
   if( strlen(token) > 6 )
      strncpy(Mptr->cloudGroup[next].other_cld_phenom,token+6,
              (strlen(token)-6));
 
   strncpy(Mptr->cloudGroup[next].cloud_type,token,3);
 
   strncpy(Mptr->cloudGroup[next].cloud_hgt_char,token+3,3);
 
   Mptr->cloudGroup[next].cloud_hgt_meters =
                               CodedHgt2Meters( token+3, Mptr );
 
   return;
}
 
 
/********************************************************************/
/*                                                                  */
/*  Title:         isSkyConditions                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

static MDSP_BOOL isSkyConditions( char **skycond, Decoded_METAR *Mptr,
                        int *NDEX )
{
 
   MDSP_BOOL first_layer,
        second_layer,
        third_layer,
        fourth_layer,
        fifth_layer,
        sixth_layer;
   int next;
 
      // interrogate skycond to determine if "CLR" is present
 
   if( *skycond == NULL )
      return FALSE;
 
 
   if( strcmp(*skycond,"CLR") == 0)
   {
      strcpy(Mptr->cloudGroup[0].cloud_type,"CLR");
/*
      memset(Mptr->cloudGroup[0].cloud_hgt_char,'\0',1);
      memset(Mptr->cloudGroup[0].other_cld_phenom,
              '\0', 1);
*/
      (*NDEX)++;
      return TRUE;
   }
 
      // interrogate skycond to determine if "SKC" is present
 
   else if( strcmp(*skycond,"SKC") == 0)
   {
      strcpy(Mptr->cloudGroup[0].cloud_type,"SKC");
/*
      memset(Mptr->cloudGroup[0].cloud_hgt_char,'\0',1);
      memset(Mptr->cloudGroup[0].other_cld_phenom,
              '\0', 1);
*/
      (*NDEX)++;
      return TRUE;
   }
 
      // interrogate skycond to determine if "NSC" is present
 
   else if( strcmp(*skycond,"NSC") == 0)
   {
      strcpy(Mptr->cloudGroup[0].cloud_type,"NSC");
/*
      memset(Mptr->cloudGroup[0].cloud_hgt_char,'\0',1);
      memset(Mptr->cloudGroup[0].other_cld_phenom,
              '\0', 1);
*/
      (*NDEX)++;
      return TRUE;
   }
 
      // interrogate skycond to determine if
      //    vertical visibility is present
 
   else if( strncmp(*skycond,"VV",2) == 0
             && strlen(*skycond) == 5 &&
                  nisdigit((*skycond+2),3) )
   {
      Mptr->VertVsby = CodedHgt2Meters( (*skycond+2), Mptr);
      strncpy(Mptr->cloudGroup[0].cloud_type,*skycond,2);
      (*NDEX)++;
      return TRUE;
   }
 
      // interrogate skycond to determine if
      //    cloud layer data is present
 
   else if( isCloudLayer( *skycond ))
   {
      next = 0;
 
      parseCloudData( *skycond , Mptr, next );
      first_layer = TRUE;
      next++;
      (++skycond);
 
      if( *skycond == NULL )
         return TRUE;
 
      second_layer = FALSE;
      third_layer = FALSE;
      fourth_layer = FALSE;
      fifth_layer = FALSE;
      sixth_layer = FALSE;
 
 
      if( isCloudLayer( *skycond ) && first_layer )
      {
         parseCloudData( *skycond, Mptr, next );
         second_layer = TRUE;
         next++;
         (++skycond);
 
         if( *skycond == NULL )
            return TRUE;
 
      }
 
      if( isCloudLayer( *skycond ) && first_layer &&
          second_layer )
      {
         parseCloudData( *skycond , Mptr, next );
         third_layer = TRUE;
         next++;
         (++skycond);
 
         if( *skycond == NULL )
            return TRUE;
 
      }
 
      if( isCloudLayer( *skycond ) && first_layer && second_layer &&
                      third_layer )
      {
         parseCloudData( *skycond, Mptr, next );
         fourth_layer = TRUE;
         next++;
         (++skycond);
 
         if( *skycond == NULL )
            return TRUE;
 
      }
 
      if( isCloudLayer( *skycond ) && first_layer && second_layer &&
          third_layer && fourth_layer )
      {
         parseCloudData( *skycond , Mptr, next );
         fifth_layer = TRUE;
         next++;
         (++skycond);
 
         if( *skycond == NULL )
            return TRUE;
 
      }
 
      if( isCloudLayer( *skycond ) && first_layer && second_layer &&
          third_layer && fourth_layer && fifth_layer )
      {
         parseCloudData( *skycond , Mptr, next );
         sixth_layer = TRUE;
      }
 
 
 
      if( sixth_layer )
      {
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( fifth_layer )
      {
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( fourth_layer )
      {
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( third_layer )
      {
         (*NDEX)++;
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( second_layer )
      {
         (*NDEX)++;
         (*NDEX)++;
         return TRUE;
      }
      else if( first_layer )
      {
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
 
   }
   else
      return FALSE;
}

/********************************************************************/
/*                                                                  */
/*  Title:         prevailingVisibility                             */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

static float prevailingVisibility( char *visibility )
{
   float Miles_vsby;
   char *temp,
        *Slash_ptr,
        *SM_KM_ptr;
   char numerator[3],
        denominator[3];

   if( (SM_KM_ptr = strstr( visibility, "SM" )) == NULL )
      SM_KM_ptr = strstr(visibility, "KM");
 
   Slash_ptr = strchr( visibility, '/' );
 
   if( Slash_ptr == NULL )
   {
      temp = (char *) malloc(sizeof(char) *
                          ((SM_KM_ptr - visibility)+1));
      memset( temp, '\0', (SM_KM_ptr-visibility)+1);
      strncpy( temp, visibility, (SM_KM_ptr-visibility) );
      Miles_vsby = (float) (atoi(temp));
      free( temp );
      return Miles_vsby;
   }
   else
   {
      memset(numerator,   '\0', sizeof(numerator));
      memset(denominator, '\0', sizeof(denominator));
 
      strncpy(numerator, visibility, (Slash_ptr - visibility));
 
/*>>>>>>>>>>>>>>>>>>>>>>
      if( (SM_KM_ptr - (Slash_ptr+1)) == 0 )
         strcpy(denominator, "4");
      else
<<<<<<<<<<<<<<<<<<<<<<*/
 
      strncpy(denominator,
              Slash_ptr+1, (SM_KM_ptr - Slash_ptr));
 
      return ( ((float)(atoi(numerator)))/
               ((float)(atoi(denominator))) );
   }
 
}

/*
 * Occasionally, visibility strings are preceded by "M ", indicating
 * that the actual visibility is something less than the value
 * in question, eg, M 1/4SM = 'less than 0.25 sm'
 */
static MDSP_BOOL isMBeforeVis( char **m, Decoded_METAR *Mptr,
                               int *NDEX )
{
    (void)Mptr;

    /*
     * NB: If the visibility string is not preceded by a token with just
     *     an M, the M may be part of the visibility string itself.
     *     That case is handled with the rest of the visibility string
     *     code.
     */

    if (*m == NULL) {
        return FALSE;
    }

    if (strncmp(*m, "M ", 2) == 0 || strncmp(*m, "< ", 2) == 0) {
        (*NDEX)++;
        return TRUE;
    }
    return FALSE;
}

/********************************************************************/
/*                                                                  */
/*  Title:         isVisibility                                     */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isVisibility( char **visblty, Decoded_METAR *Mptr,
                          int *NDEX )
{
   char *achar,
        *astring,
        *save_token;
   MDSP_BOOL isVisAfterM;

   if( *visblty == NULL )
       return FALSE;

   /*
    * Some visibility strings start with 'M' to indicate that they
    * are actually some value less than the stated quantity, ie,
    * < 1/4 SM is 'M1/4SM'. Rarely, '<' appears in place of 'M'.
    */
   if (*visblty[0] == 'M' || *visblty[0] == '<') {
       visblty[0]++;
       isVisAfterM = isVisibility(visblty, Mptr, NDEX);
       visblty[0]--;
       if (isVisAfterM) {
           Mptr->visibilityIsUpperBound = TRUE;
           return isVisAfterM;
       }
       return isVisAfterM;
   }

   // CHECK FOR VISIBILITY MISSING
   // INDICATOR FROM AUTO MEASUREMENTS

   if(strlen(*visblty) == 4 && charcmp(*visblty,"'/''/''/''/'")) {
      (*NDEX)++;
      return TRUE;
   }
 
   // CHECK FOR VISIBILITY MEASURED IN KILOMETERS
 
   if( (achar = strstr(*visblty, "KM")) != NULL )
   {
      if( nisdigit(*visblty,(achar - *visblty)) &&
                        (achar - *visblty) > 0 )
      {
         Mptr->prevail_vsbyKM = prevailingVisibility( *visblty );
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
 
   // CHECK FOR VISIBILITY MEASURED
   // IN A FRACTION OF A STATUTE MILE
 
   else if( (achar = strchr( *visblty, '/' )) !=
                    NULL &&
       (astring = strstr( *visblty, "SM")) != NULL )
   {
      if( nisdigit(*visblty,(achar - *visblty))
                     &&
               (achar - *visblty) > 0 &&
               (astring - (achar+1)) > 0 &&
                nisdigit(achar+1, (astring - (achar+1))) )
      {
         Mptr->prevail_vsbySM = prevailingVisibility (*visblty);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
 
   // check for visibility measured
   //     in whole statute miles
 
   else if( (astring = strstr(*visblty,"SM") ) != NULL )
   {
      if( nisdigit(*visblty,(astring - *visblty)) &&
                       (astring- *visblty) > 0 )
      {
         Mptr->prevail_vsbySM = prevailingVisibility (*visblty);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }

   // check for visibility measured
   // in whole and fractional statute miles
 
   else if( nisdigit( *visblty,
               strlen(*visblty)) &&
                            strlen(*visblty) < 4 )
   {
      save_token = (char *) malloc(sizeof(char)*
                              (strlen(*visblty)+1));
      strcpy(save_token,*visblty);
      if( *(++visblty) == NULL)
      {
         free( save_token );
         return FALSE;
      }
 
      if( (achar = strchr( *visblty, '/' ) ) != NULL &&
          (astring = strstr( *visblty, "SM") ) != NULL  )
      {
         if( nisdigit(*visblty,
                 (achar - *visblty)) &&
                 (achar - *visblty) > 0 &&
                 (astring - (achar+1)) > 0 &&
             nisdigit(achar+1, (astring - (achar+1))) )
         {
            Mptr->prevail_vsbySM = prevailingVisibility (*visblty);
            Mptr->prevail_vsbySM +=
                                 (float) (atoi(save_token));
            free( save_token);
 
            (*NDEX)++;
            (*NDEX)++;
 
            return TRUE;
 
         }
         else
            return FALSE;
      }
      else
         return FALSE;
 
   }
 
   // check for visibility measured
   // in meters with or without direction of observation
 
   else if( nisdigit(*visblty,4) &&
                strlen(*visblty) >= 4)
   {
      if( strcmp(*visblty+4,"NE") == 0 )
      {
         memset(Mptr->vsby_Dir, '\0', sizeof(Mptr->vsby_Dir));
         strcpy(Mptr->vsby_Dir, *visblty+4);
      }
      if( strcmp(*visblty+4,"NW") == 0 )
      {
         memset(Mptr->vsby_Dir, '\0', sizeof(Mptr->vsby_Dir));
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"SE") == 0 )
      {
         memset(Mptr->vsby_Dir, '\0', sizeof(Mptr->vsby_Dir));
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"SW") == 0 )
      {
         memset(Mptr->vsby_Dir, '\0', sizeof(Mptr->vsby_Dir));
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"N") == 0 )
      {
         memset(Mptr->vsby_Dir, '\0', sizeof(Mptr->vsby_Dir));
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"S") == 0 )
      {
         memset(Mptr->vsby_Dir, '\0', sizeof(Mptr->vsby_Dir));
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"E") == 0 )
      {
         memset(Mptr->vsby_Dir, '\0', sizeof(Mptr->vsby_Dir));
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
      if( strcmp(*visblty+4,"W") == 0 )
      {
         memset(Mptr->vsby_Dir, '\0', sizeof(Mptr->vsby_Dir));
         strcpy(Mptr->vsby_Dir,*visblty+4);
      }
 
      if( antoi(*visblty,
                  strlen(*visblty)) >= 50 &&
               antoi(*visblty,
                  strlen(*visblty)) <= 500 &&
              (antoi(*visblty,
                  strlen(*visblty)) % 50) == 0 )
      {
         Mptr->prevail_vsbyM =
           (float) (antoi(*visblty,
                       strlen(*visblty)));
         (*NDEX)++;
         return TRUE;
      }
      else if( antoi(*visblty,
                 strlen(*visblty)) >= 500 &&
           antoi(*visblty,
                 strlen(*visblty)) <= 3000 &&
          (antoi(*visblty,
                 strlen(*visblty)) % 100) == 0 )
      {
         Mptr->prevail_vsbyM =
            (float) (antoi(*visblty,
                      strlen(*visblty)));
         (*NDEX)++;
         return TRUE;
      }
      else if( antoi(*visblty,
              strlen(*visblty)) >= 3000 &&
          antoi(*visblty,
              strlen(*visblty)) <= 5000 &&
          (antoi(*visblty,
                  strlen(*visblty)) % 500) == 0 )
      {
         Mptr->prevail_vsbyM =
               (float) (antoi(*visblty,
                    strlen(*visblty)));
         (*NDEX)++;
         return TRUE;
      }
      else if(( antoi(*visblty,
            strlen(*visblty)) >= 5000 &&
          antoi(*visblty,
            strlen(*visblty)) <= 9999 &&
          (antoi(*visblty,
            strlen(*visblty)) % 500) == 0) ||
           antoi(*visblty,
            strlen(*visblty)) == 9999 )
      {
         Mptr->prevail_vsbyM =
                (float) (antoi(*visblty,
                     strlen(*visblty)));
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
 
   }
   else
      return FALSE;
 
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         variableVisibility                               */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

static MDSP_BOOL variableVisibility( char *string1, char *string2,
                      Decoded_METAR *Mptr, int *NDEX )
{
   char buf[ 32 ];
   int numerator,
       denominator;
   char *slash,
        *V_char,
        *temp;
 
   if( string1 == NULL )
      return FALSE;
 
   V_char = strchr(string1,'V');
   slash =  strchr(string1,'/');
 
   if(slash == NULL)
   {
      if(nisdigit(string1,V_char-string1))
      {
         memset(buf, '\0', sizeof(buf));
         strncpy(buf, string1, V_char-string1);
 
         if( Mptr->minVsby != (float) MAXINT )
            Mptr->minVsby += (float) atoi(buf);
         else
            Mptr->minVsby  = (float) atoi(buf);
 
         memset(buf, '\0', sizeof(buf));
         strncpy(buf, V_char+1, 5);
         Mptr->maxVsby = (float) atoi(buf);
 
      }
      else
         return FALSE;
   }
   else
   {
      temp = (char *) malloc(sizeof(char)*((V_char-string1)+1));
      memset(temp, '\0', (V_char-string1) +1);
      strncpy(temp, string1, V_char-string1);
      if( Mptr->minVsby != MAXINT )
         Mptr->minVsby += fracPart(temp);
      else
         Mptr->minVsby = fracPart(temp);
 
      free( temp );
 
      if( strchr(V_char+1,'/') != NULL)
         Mptr->maxVsby = fracPart(V_char+1);
      else
         Mptr->maxVsby = (float) atoi(V_char+1);
   }
 
   if( string2 == NULL )
      return TRUE;
   else
   {
      slash = strchr( string2, '/' );
 
      if( slash == NULL )
         return TRUE;
      else
      {
         if( nisdigit(string2,slash-string2) &&
             nisdigit(slash+1,strlen(slash+1)) )
         {
            Mptr->maxVsby += fracPart(string2);
            (*NDEX)++;
         }
         return TRUE;
      }
   }
 
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isMinMaxWinDir                                   */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

static MDSP_BOOL isMinMaxWinDir( char *string, Decoded_METAR *Mptr,
     int *NDEX )
{
#define buf_len 50
   char buf[ buf_len ];
   char *V_char;
 
   if( string == NULL )
      return FALSE;
 
   if( (V_char = strchr(string,'V')) == NULL )
      return FALSE;
   else
   {
      if( nisdigit(string,(V_char - string)) &&
               nisdigit(V_char+1,3) )
      {
         memset( buf, '\0', buf_len);
         strncpy( buf, string, V_char - string);
         Mptr->minWnDir = atoi( buf );
 
         memset( buf, '\0', buf_len);
         strcpy( buf, V_char+1 );
         Mptr->maxWnDir = atoi( buf );
 
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
}

/********************************************************************/
/*                                                                  */
/*  Title:         isRVR                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isRVR( char *token, Decoded_METAR *Mptr, int *NDEX,
                     int ndex )
{
   char *slashPtr, *FT_ptr;
   char *vPtr;
   int length;
 
   if( token == NULL )
      return FALSE;
 
   if( *token != 'R' || (length = strlen(token)) < 7 ||
        (slashPtr = strchr(token,'/')) == NULL ||
        nisdigit(token+1,2) == FALSE )
      return FALSE;
 
   if( (slashPtr - (token+3)) > 0 )
      if( !nisalpha(token+3,(slashPtr - (token+3))) )
         return FALSE;
 
   if( strcmp(token+(strlen(token)-2),"FT") != 0 ) {
      // The unit is now meters.
      Mptr->RRVR[ndex].distance_unit = DIST_METERS;
      FT_ptr = token + strlen(token);
   }
   else
      FT_ptr = token + (strlen(token)-2);
 
   if( strchr(slashPtr+1, 'P' ) != NULL )
      Mptr->RRVR[ndex].above_max_RVR = TRUE;
 
   if( strchr(slashPtr+1, 'M' ) != NULL )
      Mptr->RRVR[ndex].below_min_RVR = TRUE;
 
 
   strncpy(Mptr->RRVR[ndex].runway_designator, token+1,
           (slashPtr-(token+1)));
 
   if( (vPtr = strchr(slashPtr, 'V' )) != NULL )
   {
      Mptr->RRVR[ndex].vrbl_visRange = TRUE;
      Mptr->RRVR[ndex].Min_visRange = antoi(slashPtr+1,
                              (vPtr-(slashPtr+1)) );

      /* if the style was something like 5000VP6000, skip the P
       * before looking for the max
       */
      if (isalpha(*(vPtr + 1))) {
	  vPtr++;
      }
      Mptr->RRVR[ndex].Max_visRange = antoi(vPtr+1,
                              (FT_ptr - (vPtr+1)) );
      (*NDEX)++;
      return TRUE;
   }
   else
   {
      if( Mptr->RRVR[ndex].below_min_RVR ||
          Mptr->RRVR[ndex].above_max_RVR    )
         Mptr->RRVR[ndex].visRange = antoi(slashPtr+2,
                           (FT_ptr - (slashPtr+2)) );
      else
         Mptr->RRVR[ndex].visRange = antoi(slashPtr+1,
                           (FT_ptr - (slashPtr+1)) );
 
      (*NDEX)++;
      return TRUE;
   }
 
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isAltimeterString                                */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isAltimeterString( char *token, Decoded_METAR *Mptr, int *NDEX )
{
   char dummy[6];
 
 
   if( token == NULL )
      return FALSE;
 
   if( strlen(token) < 5 )
      return FALSE;
   else
   {
      Mptr->A_altstng = FALSE;
      Mptr->Q_altstng = FALSE;
 
      if( (*token == 'A' || *token == 'Q') &&
           (nisdigit(token+1, strlen(token)-1) ||
            nisdigit(token+1,strlen(token)-3)) )
      {
         if( *token == 'A' )
         {
            Mptr->A_altstng = TRUE;
            Mptr->inches_altstng = atof(token+1) * 0.01;
         }
         else
         {
            Mptr->Q_altstng = TRUE;
 
            if( strchr(token,'.') != NULL)
            {
               memset(dummy, '\0',  sizeof(dummy));
               strncpy(dummy,token+1,4);
               Mptr->hectoPasc_altstng = atoi(dummy);
            }
            else
               Mptr->hectoPasc_altstng = atoi(token+1);
         }
 
         (*NDEX)++;
         return TRUE;
 
      }
      return FALSE;
   }
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isTempGroup                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isTempGroup( char *token, Decoded_METAR *Mptr, int *NDEX)
{
 
   /***************************/
   /* DECLARE LOCAL VARIABLES */
   /***************************/
 
   char *slash;
 
   if( token == NULL )
      return FALSE;
 
   if( (slash = strchr(token,'/')) == NULL)
      return FALSE;
   else
   {
      if( charcmp(token,"aa'/'dd") ) {
         Mptr->dew_pt_temp = atoi(slash+1);
         (*NDEX)++;
         return TRUE;
      }
      else if( charcmp(token,"aa'/''M'dd") ) {
         Mptr->dew_pt_temp = atoi(slash+2) * -1;
         (*NDEX)++;
         return TRUE;
      }
      else if( charcmp(token,"dd'/'aa") ) {
         Mptr->temp = antoi(token,(slash-token));
         (*NDEX)++;
         return TRUE;
      }
      else if( charcmp(token,"'M'dd'/'aa") ) {
         Mptr->temp = antoi(token+1,(slash-(token+1))) * -1;
         (*NDEX)++;
         return TRUE;
      }
      else if( nisdigit(token,(slash-token)) &&
           nisdigit(slash+1,strlen(slash+1)) )
      {
         Mptr->temp = antoi(token,(slash-token));
         Mptr->dew_pt_temp = atoi(slash+1);
         (*NDEX)++;
         return TRUE;
      }
      else if( *token == 'M' && nisdigit(token+1,(slash-(token+1)))
                && *(slash+1) != '\0' &&
            *(slash+1) == 'M' && nisdigit(slash+2,strlen(slash+2)) )
      {
         Mptr->temp = antoi(token+1,(slash-(token+1))) * -1;
         Mptr->dew_pt_temp = atoi(slash+2) * -1;
         (*NDEX)++;
         return TRUE;
      }
      else if( *token == 'M' && nisdigit(token+1,(slash-(token+1)))
                 && *(slash+1) != '\0' &&
               nisdigit(slash+1,strlen(slash+1)) )
      {
         Mptr->temp = antoi(token+1,(slash-(token+1))) * -1;
         Mptr->dew_pt_temp = atoi(slash+1);
         (*NDEX)++;
         return TRUE;
      }
      else if( nisdigit(token,(slash - token)) &&
                    *(slash+1) != '\0' &&
                    nisdigit(slash+2,strlen(slash+2)) )
      {
         Mptr->temp = antoi(token,(slash-token));
         Mptr->dew_pt_temp = atoi(slash+2) * -1;
         (*NDEX)++;
         return TRUE;
      }
      else if( nisdigit(token,(slash-token)) &&
           strlen(token) <= 3)
      {
         Mptr->temp = antoi(token,(slash-token));
         (*NDEX)++;
         return TRUE;
      }
      else if( *token == 'M' &&
                   nisdigit(token+1,(slash-(token+1))) &&
                   strlen(token) <= 4)
      {
         Mptr->temp = antoi(token+1,(slash-(token+1))) * -1;
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
 
}
 
 
/********************************************************************/
/*                                                                  */
/*  Title:         isWxToken                                        */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isWxToken( char *token )
{
   int i;
 
   if( token == NULL )
      return FALSE;
   for( i = 0; i < strlen(token); i++ )
   {
      if( !(isalpha(*(token+i)) || *(token+i) == '+' ||
                                   *(token+i) == '-' ||
                                   *(token+i) == '/'  ) )
         return FALSE;
   }
   return TRUE;
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isPresentWX                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isPresentWX( char *token, Decoded_METAR *Mptr,
                        int *NDEX, int *next )
{
   static char *WxSymbols[] = {"BCFG", "BLDU", "BLSA", "BLPY",
          "BLSN", "FZBR", "VCBR", "TSGR", "VCTS",
          "DRDU", "DRSA", "DRSN", "FZFG", "FZDZ", "FZRA",
          "PRFG", "MIFG",
          "SHRA", "SHSN", "SHPE", "SHPL", "SHGS",
          "SHGR",
          "VCFG", "VCFC",
          "VCSS", "VCDS", "TSRA", "TSPE", "TSPL", "TSSN",
          "VCSH", "VCPO", "VCBLDU", "VCBLSA", "VCBLSN",
 
          "BR", "DU",
          "DZ", "DS",
          "FG", "FC", "FU", "GS", "GR", "HZ", "IC",
          "PE", "PL", "PO", "RA",
          "SN", "SG", "SQ", "SA", "SS", "TS",
          "VA",
          "PY", NULL};
 
   int i;
   char *ptr,
        *temp_token,
        *save_token,
        *temp_token_orig;
 
   if( token == NULL)
      return FALSE;

   if(strlen(token) == 2 && charcmp(token,"'/''/'")) {
      strcpy( Mptr->WxObstruct[*next], "MISSING");
      (*next)++;
      (*NDEX)++;
      return TRUE;
   }
 
   temp_token_orig = temp_token =
        (char *) malloc(sizeof(char)*(strlen(token)+1));
   strcpy(temp_token, token);
   while( temp_token != NULL && (*next) < MAXWXSYMBOLS )
   {
      i = 0;
      save_token = NULL;
 
      if( *temp_token == '+' || *temp_token == '-' )
      {
         save_token = temp_token;
         temp_token++;
      }
 
      while( WxSymbols[i] != NULL )
         if( strncmp(temp_token, WxSymbols[i],
                      strlen(WxSymbols[i])) != 0 )
            i++;
         else
            break;
 
      if( WxSymbols[i] == NULL ) {
         free( temp_token_orig );
         return FALSE;
      }
      else
      {
 
         if( save_token != NULL )
         {
            strncpy( Mptr->WxObstruct[*next], save_token, 1);
            strcpy( (Mptr->WxObstruct[*next])+1,
                              WxSymbols[i]);
            (*next)++;
         }
         else
         {
            strcpy( Mptr->WxObstruct[*next], WxSymbols[i]);
            (*next)++;
         }
 
 
         if( strcmp(temp_token, WxSymbols[i]) != 0)
         {
            ptr = strstr(temp_token, WxSymbols[i]);
            temp_token = ptr + strlen(WxSymbols[i]);
         }
         else
         {
            free( temp_token_orig );
            temp_token = NULL;
            (*NDEX)++;
            return TRUE;
         }
 
      }
 
   }
 
   free( temp_token_orig );
   return FALSE;
 
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isStnID                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isStnId( char *stnID, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( stnID == NULL )
      return FALSE;
 
#ifdef CMCPRT
   printf("isStnId:  stnID = %s\n",stnID);
#endif
 
   if( strlen(stnID) == 4 )
   {
      if( nisalpha(stnID,1) != 0 && nisalnum(stnID+1,3) != 0 ) {
         strcpy(Mptr->stnid,stnID);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   else
      return FALSE;
 
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isCodeName                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isCodeName( char *codename, Decoded_METAR *Mptr, int *NDEX)
{
   if( codename == NULL )
      return FALSE;
 
   if( strcmp(codename,"METAR") == 0 ||
       strcmp(codename,"SPECI") == 0   )
   {
      strcpy(Mptr->codeName, codename );
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
 
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isNIL                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isNIL( char *token, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( token == NULL )
      return FALSE;
 
   if( strcmp(token, "NIL") == 0 )
   {
      Mptr->NIL_rpt = TRUE;
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
 
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isAUTO                                           */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isAUTO( char *token, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( token == NULL )
      return FALSE;
 
   if( strcmp(token, "AUTO") == 0 )
   {
      Mptr->AUTO = TRUE;
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
 
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isCOR                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          24 Apr 1996                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isCOR ( char *token, Decoded_METAR *Mptr, int *NDEX)
{
 
   if( token == NULL )
      return FALSE;
 
   if( strcmp(token, "COR") == 0 )
   {
      Mptr->COR  = TRUE;
      (*NDEX)++;
      return TRUE;
   }
   else
      return FALSE;
 
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isTimeUTC                                        */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isTimeUTC( char *UTC, Decoded_METAR *Mptr, int *NDEX )
{
 
   if( UTC == NULL )
      return FALSE;
 
   if( strlen( UTC ) == 4 ) {
      if(nisdigit(UTC,4) ) {
         Mptr->ob_hour = antoi(UTC,2);
         Mptr->ob_minute = antoi(UTC+2,2);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   else if( strlen( UTC ) == 6 ) {
      if(nisdigit(UTC,6) ) {
         Mptr->ob_date = antoi(UTC,2);
         Mptr->ob_hour = antoi(UTC+2,2);
         Mptr->ob_minute = antoi(UTC+4,2);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   if( strlen( UTC ) == 5 ) {
      if(nisdigit(UTC,4) && (*(UTC+4) == 'Z') ) {
         Mptr->ob_hour = antoi(UTC,2);
         Mptr->ob_minute = antoi(UTC+2,2);
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   else if( strlen( UTC ) == 7 ) {
      if(nisdigit(UTC,6) && (*(UTC+6) == 'Z') ) {
         Mptr->ob_date = antoi(UTC,2);
         Mptr->ob_hour = antoi(UTC+2, 2);
         Mptr->ob_minute = antoi(UTC+4, 2 );
         (*NDEX)++;
         return TRUE;
      }
      else
         return FALSE;
   }
   else
      return FALSE;
}

/********************************************************************/
/*                                                                  */
/*  Title:         defaultWindUnitsForAirport                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

static char* defaultWindUnitsForAirport( char *airportId )
{
   /* Airport prefix Z covers Chinese airports which use MPS */
   /* Airport prefix U covers Russia and the former Soviet states airports. */
   /* They use both MPS and KT so we cannot determine the units based on that prefix. */
   int i;
   static char *airportPrefixUsingMPS[1] = { "Z" },
               *airportPrefixWithoutDefault[1] = { "U" };

   char airportPrefix = airportId[0];

   /* Check that an ICAO airport code has been passed in. */
   if( strlen(airportId) != 4 ) {
      return "";
   }

   for ( i = 0; i < sizeof(airportPrefixUsingMPS) / sizeof(airportPrefixUsingMPS[0]); i++ ) {
      if (strncmp(airportId,airportPrefixUsingMPS[i],1) == 0) {
         return "MPS";
      }
   }

   for ( i = 0; i < sizeof(airportPrefixWithoutDefault) / sizeof(airportPrefixWithoutDefault[0]); i++ ) {
      if (strncmp(airportId,airportPrefixWithoutDefault[i],1) == 0) {
         return "";
      }
   }

   return "KT";
}
 
/********************************************************************/
/*                                                                  */
/*  Title:         isWindData                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
static MDSP_BOOL isWindData( char *wind, Decoded_METAR *Mptr, int *NDEX )
{
 
   char *GustPtr,
        *unitsPtr,
        *defaultUnit;
   char dummy[8];
 
   if( wind == NULL )
      return FALSE;

   if(strlen(wind) == 5 && charcmp(wind,"'/''/''/''/''/'")) {
      (*NDEX)++;
      return TRUE;
   }
 
   if( strlen(wind) < 5 )
      return FALSE;
 
   memset(dummy,'\0', sizeof(dummy));
 
   /* CHECK FOR VARIABLE ("VRB") WIND SPEED */
 
   if( charcmp(wind,"'V''R''B'dd'K''T'")) {
      Mptr->winData.windVRB = TRUE;
      Mptr->winData.windSpeed = antoi(wind+3,2);
      memset(Mptr->winData.windUnits, '\0', sizeof(Mptr->winData.windUnits));
      strcpy(Mptr->winData.windUnits,"KT");
      (*NDEX)++;
/*
printf("isWindData:  Passed VRBddKT test - wind = %s\n",wind);
*/
      return TRUE;
   }
 
   if( charcmp(wind,"'V''R''B'ddd'K''T'")) {
      Mptr->winData.windVRB = TRUE;
      Mptr->winData.windSpeed = antoi(wind+3,3);
      memset(Mptr->winData.windUnits, '\0', sizeof(Mptr->winData.windUnits));
      strcpy(Mptr->winData.windUnits,"KT");
      (*NDEX)++;
/*
printf("isWindData:  Passed VRBdddKT test - wind = %s\n",wind);
*/
      return TRUE;
   }
 
   if( charcmp(wind,"'V''R''B'ddd'G'ddd'K''T'")) {
      Mptr->winData.windVRB = TRUE;
      Mptr->winData.windSpeed = antoi(wind+3,3);
      Mptr->winData.windGust = antoi(wind+7,3);
 
      memset(Mptr->winData.windUnits, '\0', sizeof(Mptr->winData.windUnits));
      strcpy(Mptr->winData.windUnits,"KT");
      (*NDEX)++;
      return TRUE;
   }
 
   if( charcmp(wind,"'V''R''B'dd'G'dd'K''T'")) {
      Mptr->winData.windVRB = TRUE;
      Mptr->winData.windSpeed = antoi(wind+3,2);
      Mptr->winData.windGust = antoi(wind+6,2);
 
      memset(Mptr->winData.windUnits, '\0', sizeof(Mptr->winData.windUnits));
      strcpy(Mptr->winData.windUnits,"KT");
      (*NDEX)++;
      return TRUE;
   }
 
   if( charcmp(wind,"'V''R''B'dd'G'ddd'K''T'")) {
      Mptr->winData.windVRB = TRUE;
      Mptr->winData.windSpeed = antoi(wind+3,2);
      Mptr->winData.windGust = antoi(wind+6,3);
 
      memset(Mptr->winData.windUnits, '\0', sizeof(Mptr->winData.windUnits));
      strcpy(Mptr->winData.windUnits,"KT");
      (*NDEX)++;
      return TRUE;
   }

   /* CHECK FOR WIND SPEED UNITS */
    
   /*
      if( ( unitsPtr = strstr( wind, "KMH" ) ) != NULL )
         strcpy( dummy, "KMH" );
      else if( (unitsPtr = strstr( wind, "MPS") ) != NULL )
         strcpy( dummy, "MPS" );
   */
    
   if( (unitsPtr = strstr( wind, "KT") ) != NULL ) {
      strcpy( dummy, "KT" );
   }
   else if( (unitsPtr = strstr( wind, "MPS") ) != NULL) {
      strcpy( dummy, "MPS" );
   }
   else if ( charcmp(wind,"ddddd'G'dd") || charcmp(wind,"ddddd") ) {
      unitsPtr = wind + strlen(wind);
      strcpy( dummy, defaultWindUnitsForAirport(Mptr->stnid));
   }
   else
      return FALSE;

   // check for wind gusts
 
   if( (GustPtr = strchr( wind, 'G' )) != NULL )
   {
/*
printf("isWindData:  Passed 1st GUST test - wind = %s\n",wind);
*/
      if( nisdigit(wind,(GustPtr-wind)) &&
            nisdigit(GustPtr+1,(unitsPtr-(GustPtr+1))) &&
            ((GustPtr-wind) >= 5 && (GustPtr-wind) <= 6) &&
            ((unitsPtr-(GustPtr+1)) >= 2 &&
             (unitsPtr-(GustPtr+1)) <= 3) )
      {
         Mptr->winData.windDir = antoi(wind,3);
 
         Mptr->winData.windSpeed = antoi(wind+3, (GustPtr-(wind+3)));
         Mptr->winData.windGust = antoi(GustPtr+1,(unitsPtr-
                                                    (GustPtr+1)));
         strcpy( Mptr->winData.windUnits, dummy );
/*
printf("isWindData:  Passed 2nd GUST test - wind = %s\n",wind);
*/
         (*NDEX)++;
         return TRUE;
      }
      else {
/*
printf("isWindData:  Failed 2nd GUST test - wind = %s\n",wind);
*/
         return FALSE;
      }
   }
   else if( nisdigit(wind,(unitsPtr-wind)) &&
            ((unitsPtr-wind) >= 5 && (unitsPtr-wind) <= 6) )
   {
      Mptr->winData.windDir = antoi(wind, 3);
 
      Mptr->winData.windSpeed = antoi(wind+3,(unitsPtr-(wind+3)));
      strcpy( Mptr->winData.windUnits, dummy );
      (*NDEX)++;
/*
printf("isWindData:  Passed dddff(f) test - wind = %s\n",wind);
*/
      return TRUE;
   }
   else {
      return FALSE;
   }
 
}

/********************************************************************/
/*                                                                  */
/*  Title:         decode_metar                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          14 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      decode_metar takes a pointer to a METAR report char- */
/*                 acter string as input, decodes the report, and   */
/*                 puts the individual decoded/parsed groups into   */
/*                 a structure that has the variable type           */
/*                 Decoded_METAR.                                   */
/*                                                                  */
/*  Input:         string - a pointer to a METAR report character   */
/*                          string.                                 */
/*                                                                  */
/*  Output:        Mptr   - a pointer to a structure that has the   */
/*                          variable type Decoded_METAR.            */
/*                                                                  */
/*  Modification History:                                           */
/*                 3 Jul 2001 by Eric McCarthy: Added stringCpy     */
/*                     so const char *'s could be passed in.        */
/*                                                                  */
/********************************************************************/
 
int decode_metar( char *string , Decoded_METAR *Mptr )
{
   /* DECLARE LOCAL VARIABLES */
 
   enum METAR_obGroup { codename, stnid, NIL1, COR1, obDateTime, NIL2,
                        AUTO, COR, windData, MinMaxWinDir,
                        CAVOK, visibility,
                        RVR, presentWX, PartialObscur,
                        skyCond, tempGroup,
                        altimStng, NotIDed = 99} StartGroup,
                                      SaveStartGroup,
                                      MetarGroup;
 
   WindStruct *WinDataPtr;
 
   int    ndex,
          NDEX,
          i,
          jkj,
          j;
 
 
   char   **token,
          *delimeters = {" "},
          *stringCpy;
 
   MDSP_BOOL IS_NOT_RMKS;
 
   /* ONLY PARSE OR DECOCODE NON-NULL METAR REPORT STRINGS */
 
   if( string == NULL || *string == '\0' )
      return 8;
 
 
   /*   INITIALIZE STRUCTURE THAT HAS THE   */
   /*      VARIABLE TYPE Decoded_METAR      */
 
   init_metar_decoder (Mptr);
 
#ifdef DEBUGZZ
   printf("decode_metar: Returned from init_metar_decoder\n");
#endif

	/* Copy the string since it may be const, and functions
	 * strtok() don't like that.
	 */
	
	stringCpy = calloc(strlen(string) + 1, sizeof(char));
	strcpy(stringCpy, string);

 
   /* TOKENIZE AND STORE THE INPUT METAR REPORT STRING */
#ifdef DEBUGZZ
   printf("decode_metar: Before start of tokenizing, string = %s\n",
             stringCpy);
#endif
 
   token = SaveTokenString( stringCpy, delimeters );
 
   /* DECODE THE METAR REPORT (POSITIONAL ORDER PRECEDENCE) */
 
   NDEX = 0;
   MetarGroup = codename;
   IS_NOT_RMKS = TRUE;
 
#ifdef DEBUGZZ
    printf("decode_metar: token[0] = %s\n",token[0]);
#endif
 
   while( token[NDEX] != NULL && IS_NOT_RMKS ) {
 
#ifdef DEBUGZZ
if( strcmp(token[0],"OPKC") == 0 || strcmp(token[0],"TAPA") == 0 ) {
   printf("decode_metar:  token[%d] = %s\n",NDEX,token[NDEX]);
   printf("decode_metar: Token[%d] = %s\n",NDEX,token[NDEX]);
   printf("decode_metar: MetarGroup = %d\n",MetarGroup);
}
#endif
 
    if( strcmp( token[NDEX], "RMK" ) != 0 ) {
 
      StartGroup = NotIDed;
 
#ifdef DEBUGZZ
if( strcmp(token[0],"OPKC") == 0 || strcmp(token[0],"TAPA") == 0 ) {
   printf("decode_metar: StartGroup = %d\n",StartGroup);
   printf("decode_metar: SaveStartGroup = %d\n",SaveStartGroup);
}
#endif
 
      /* SET ID_break_CODE TO ITS DEFAULT VALUE OF  */
      /* 99, WHICH MEANS THAT NO SUCCESSFUL ATTEMPT */
      /* WAS MADE TO DECODE ANY METAR CODED GROUP   */
      /* FOR THIS PASS THROUGH THE DECODING LOOP    */

      switch( MetarGroup ) {
         case( codename ):
            if( isCodeName( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = codename;
            MetarGroup = stnid;
            break;
         case( stnid ):
            if( isStnId( token[NDEX], Mptr, &NDEX ) ) {
               SaveStartGroup = StartGroup = stnid;
               MetarGroup = NIL1;
            }
            else {
#ifdef DEBUGZX
printf("decode_metar:  token[%d] = %s\n",NDEX,token[NDEX]);
#endif
               freeTokens( token );
               return 12;
            }
            break;
         case( NIL1 ):
            if( isNIL( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = NIL1;
            MetarGroup = COR1;
            break;
         case( COR1 ):
            if( isCOR( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = COR1;
            MetarGroup = obDateTime;
            break;
         case( obDateTime ):
            if( isTimeUTC( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = obDateTime;
            MetarGroup = NIL2;
            break;
         case( NIL2 ):
            if( isNIL( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = NIL2;
            MetarGroup = AUTO;
            break;
         case( AUTO ):
            if( isAUTO( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = AUTO;
            MetarGroup = COR;
            break;
         case( COR ):
            if( isCOR( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = COR;
            MetarGroup = windData;
            break;
         case( windData ):
            if( isWindData( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = windData;
            MetarGroup = MinMaxWinDir;
            break;
         case( MinMaxWinDir ):
            if( isMinMaxWinDir( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = MinMaxWinDir;
            MetarGroup = CAVOK;
            break;
         case( CAVOK ):
            if( isCAVOK( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = CAVOK;
            MetarGroup = visibility;
            break;
         case( visibility ):
            if( isMBeforeVis( &(token[NDEX]), Mptr, &NDEX ) ) {
                Mptr->visibilityIsUpperBound = TRUE;
            }
            if( isVisibility( &(token[NDEX]), Mptr, &NDEX ) ) {
                SaveStartGroup = StartGroup = visibility;
            } else if (Mptr->visibilityIsUpperBound) {
                Mptr->visibilityIsUpperBound = FALSE;
                NDEX--;
            }
            MetarGroup = RVR;
            break;
         case( RVR ):
            ndex = 0;
            MetarGroup = presentWX;
 
            while (isRVR( token[NDEX], Mptr, &NDEX, ndex ) &&
                               ndex < MAX_RUNWAYS ) {
               ndex++;
               SaveStartGroup = StartGroup = RVR;
               MetarGroup = presentWX;
            }
            break;
         case( presentWX ):
            ndex = 0;
            MetarGroup = skyCond;
 
            while( isPresentWX( token[NDEX], Mptr, &NDEX,
                          &ndex ) && ndex < MAXWXSYMBOLS) {
               SaveStartGroup = StartGroup = presentWX;
               MetarGroup = PartialObscur;
            }
            break;
         case( PartialObscur ):
            if( isPartialObscuration( &(token[NDEX]), Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = PartialObscur;
            MetarGroup = skyCond;
            break;
         case( skyCond ):
            if( isSkyConditions( &(token[NDEX]), Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = skyCond;
            MetarGroup = tempGroup;
            break;
         case( tempGroup ):
            if( isTempGroup( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = tempGroup;
            MetarGroup = altimStng;
            break;
         case( altimStng ):
            if( isAltimeterString( token[NDEX], Mptr, &NDEX ) )
               SaveStartGroup = StartGroup = altimStng;
            MetarGroup = NotIDed;
            break;
         default:
            NDEX++;
/*          MetarGroup = SaveStartGroup;   */
            MetarGroup = ResetMETARGroup( StartGroup,
                                          SaveStartGroup );
            break;
      }
    }
    else
      IS_NOT_RMKS = FALSE;
 
   }
 
 
#ifdef DEBUGZZ
if( strcmp(token[0],"OPKC") == 0 || strcmp(token[0],"TAPA") == 0 ) {
   printf("decode_metar:  while loop exited, Token[%d] = %s\n",
                  NDEX,token[NDEX]);
}
#endif
	 /* DECODE GROUPS FOUND IN THE */
	 /*  REMARKS SECTION OF THE    */
	 /*       METAR REPORT         */

#ifdef PRTMETAR
printf("decode_metar:  Print DECODED METAR, before leaving "
       "decode_metar Routine, but before possible call to decode_metar_remark\n\n");
print_decoded_metar( Mptr );
#endif
 
   if( token[NDEX] != NULL )
      if( strcmp( token[NDEX], "RMK" ) == 0 )
         decode_metar_remark( token, Mptr );
 
#ifdef PRTMETAR
printf("decode_metar:  Print DECODED METAR, after possible decode_metar_remark "
       "call\n\n");
print_decoded_metar( Mptr );
#endif
 
   freeTokens( token );    /* FREE THE STORAGE ALLOCATED FOR THE   */
                           /* ARRAY USED TO HOLD THE METAR REPORT  */
                           /*                GROUPS                */
   free(stringCpy);
   
   return 0;
 
}




/********************************************************************/
/*                                                                  */
/*  Title:         decode_net_metar                                 */
/*  Date:          24 Jul 2001                                      */
/*  Programmer:    Eric McCarthy                                    */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  decode_net_metar                                     */
/*                 The METARs supplied by the NWS server need to    */
/*                 be reformatted before they can be sent through   */
/*                 decode_metar. This calls decode_metar on the     */
/*                 correctly formated METAR.                        */
/*                                                                  */
/*  Input:         a pointer to a METAR string from a NWS server    */
/*                                                                  */
/*  Output:        Mptr   - a pointer to a structure that has the   */
/*                          variable type Decoded_METAR.            */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

int decode_net_metar (char *string, Decoded_METAR *Mptr)
{
	char *string_cpy, *ptr;
	int result;
	
	/* Strip the date, which is the first line. */
	while (*string != '\n')
	{
		++string;
	}
	++string;
	
	/* make a copy of the string without the date */
	string_cpy = (char *) calloc(strlen(string), sizeof(char));
	strcpy(string_cpy, string);
	
	/* replace all carrage returns with spaces */
	ptr = string_cpy;
	while (*ptr != '\0')
	{
		if (*ptr == '\n')
			*ptr = ' ';
		++ptr;
	}
	
	/* decode the METAR */
	result = decode_metar(string_cpy, Mptr);
	
	free(string_cpy);
	return result;
}

// vim: set ts=4 sw=4 sts=4 noet :
