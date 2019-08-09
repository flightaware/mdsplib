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

#include "metar_structs.h"

char* distance_as_string(Distance_Unit unit) {
   switch (unit) {
   case DIST_FEET: return "FT";
   case DIST_METERS: return "M";
   default: return "UNKNOWN UNIT";
   }
}

/* NB this is pretty insecure insofar as it blindly writes into string
 * on the assuption that outstring can fit it all
 * */

void sprintf_tornadic_info (char * outstring, Decoded_METAR *Mptr) {
   char temp[100];

   if ( Mptr->TornadicType[0] != '\0' ) {
      sprintf(temp, "TORNADIC ACTVTY TYPE: %s\n",
         Mptr->TornadicType );
      strcat(outstring, temp);
   }

   if ( Mptr->BTornadicHour != MAXINT ) {
      sprintf(temp, "TORN. ACTVTY BEGHOUR: %d\n",
         Mptr->BTornadicHour );
      strcat(outstring, temp);
   }

   if ( Mptr->BTornadicMinute != MAXINT ) {
      sprintf(temp, "TORN. ACTVTY BEGMIN : %d\n",
         Mptr->BTornadicMinute );
      strcat(outstring, temp);
   }

   if ( Mptr->ETornadicHour != MAXINT ) {
      sprintf(temp, "TORN. ACTVTY ENDHOUR: %d\n",
         Mptr->ETornadicHour );
      strcat(outstring, temp);
   }

   if ( Mptr->ETornadicMinute != MAXINT ) {
      sprintf(temp, "TORN. ACTVTY ENDMIN : %d\n",
         Mptr->ETornadicMinute );
      strcat(outstring, temp);
   }

   if ( Mptr->TornadicDistance != MAXINT ) {
      sprintf(temp, "TORN. DIST. FROM STN: %d\n",
         Mptr->TornadicDistance );
      strcat(outstring, temp);
   }

   if ( Mptr->TornadicLOC[0] != '\0' ) {
      sprintf(temp, "TORNADIC LOCATION   : %s\n",
         Mptr->TornadicLOC );
      strcat(outstring, temp);
   }

   if ( Mptr->TornadicDIR[0] != '\0' ) {
      sprintf(temp, "TORNAD. DIR FROM STN: %s\n",
         Mptr->TornadicDIR );
      strcat(outstring, temp);
   }

   if ( Mptr->TornadicMovDir[0] != '\0' ) {
      sprintf(temp, "TORNADO DIR OF MOVM.: %s\n",
         Mptr->TornadicMovDir );
      strcat(outstring, temp);
   }
}


/* NB this is pretty insecure insofar as it blindly writes into string
 * on the assuption that outstring can fit it all
 * */

void sprint_metar (char * outstring, Decoded_METAR *Mptr)
{
   int i;
   char temp[100];

   sprintf(outstring, "\n\n\n/*******************************************/\n");
   strcat(outstring, "/*    THE DECODED METAR REPORT FOLLOWS     */\n");
   strcat(outstring, "/*******************************************/\n\n");

   if ( Mptr->codeName[ 0 ] != '\0' ) {
      sprintf(temp, "REPORT CODE NAME    : %s\n",Mptr->codeName);
      strcat(outstring, temp);
   }

   if ( Mptr->stnid[ 0 ] != '\0' ) {
      sprintf(temp, "STATION ID          : %s\n",Mptr->stnid);
      strcat(outstring, temp);
   }

   if ( Mptr->ob_date != MAXINT ) {
      sprintf(temp, "OBSERVATION DAY     : %d\n",Mptr->ob_date);
      strcat(outstring, temp);
   }

   if ( Mptr->ob_hour != MAXINT ) {
      sprintf(temp, "OBSERVATION HOUR    : %d\n",Mptr->ob_hour);
      strcat(outstring, temp);
   }

   if ( Mptr->ob_minute != MAXINT ) {
      sprintf(temp, "OBSERVATION MINUTE  : %d\n",Mptr->ob_minute);
      strcat(outstring, temp);
   }

   if ( Mptr->NIL_rpt ) {
      sprintf(temp, "NIL REPORT          : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->AUTO ) {
      sprintf(temp, "AUTO REPORT         : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->COR ) {
      sprintf(temp, "CORRECTED REPORT    : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->winData.windVRB ) {
      sprintf(temp, "WIND DIRECTION VRB  : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->winData.windDir != MAXINT ) {
      sprintf(temp, "WIND DIRECTION      : %d\n",Mptr->winData.windDir);
      strcat(outstring, temp);
   }

   if ( Mptr->winData.windSpeed != MAXINT ) {
      sprintf(temp, "WIND SPEED          : %d\n",Mptr->winData.windSpeed);
      strcat(outstring, temp);
   }

   if ( Mptr->winData.windGust != MAXINT ) {
      sprintf(temp, "WIND GUST           : %d\n",Mptr->winData.windGust);
      strcat(outstring, temp);
   }

   if ( Mptr->winData.windUnits[ 0 ] != '\0' ) {
      sprintf(temp, "WIND UNITS          : %s\n",Mptr->winData.windUnits);
      strcat(outstring, temp);
   }

   if ( Mptr->minWnDir != MAXINT ) {
      sprintf(temp, "MIN WIND DIRECTION  : %d\n",Mptr->minWnDir);
      strcat(outstring, temp);
   }

   if ( Mptr->maxWnDir != MAXINT ) {
      sprintf(temp, "MAX WIND DIRECTION  : %d\n",Mptr->maxWnDir);
      strcat(outstring, temp);
   }

   if ( Mptr->prevail_vsbyM != (float) MAXINT ) {
      sprintf(temp, "PREVAIL VSBY (M)    : %f\n",Mptr->prevail_vsbyM);
      strcat(outstring, temp);
   }

   if ( Mptr->prevail_vsbyKM != (float) MAXINT ) {
      sprintf(temp, "PREVAIL VSBY (KM)   : %f\n",Mptr->prevail_vsbyKM);
      strcat(outstring, temp);
   }

   if ( Mptr->prevail_vsbySM != (float) MAXINT ) {
      sprintf(temp, "PREVAIL VSBY (SM)   : %.3f\n",Mptr->prevail_vsbySM);
      strcat(outstring, temp);
   }

   if ( Mptr->vsby_Dir[ 0 ] != '\0' ) {
      sprintf(temp, "VISIBILITY DIRECTION: %s\n",Mptr->vsby_Dir);
      strcat(outstring, temp);
   }

   if ( Mptr->RVRNO ) {
      sprintf(temp, "RVRNO               : TRUE\n");
      strcat(outstring, temp);
   }

   for ( i = 0; i < MAX_RUNWAYS; i++ )
   {
      if( Mptr->RRVR[i].runway_designator[0] != '\0' ) {
         sprintf(temp, "RUNWAY DESIGNATOR   : %s\n",
                 Mptr->RRVR[i].runway_designator);
         strcat(outstring, temp);
      }

      if( Mptr->RRVR[i].visRange != MAXINT ) {
         sprintf(temp, "R_WAY VIS RANGE (%s): %d\n",
                 distance_as_string(Mptr->RRVR[i].distance_unit),
                 Mptr->RRVR[i].visRange);
         strcat(outstring, temp);
      }

      if ( Mptr->RRVR[i].vrbl_visRange ) {
         sprintf(temp, "VRBL VISUAL RANGE   : TRUE\n");
         strcat(outstring, temp);
      }

      if ( Mptr->RRVR[i].below_min_RVR ) {
         sprintf(temp, "BELOW MIN RVR       : TRUE\n");
         strcat(outstring, temp);
      }

      if ( Mptr->RRVR[i].above_max_RVR ) {
         sprintf(temp, "ABOVE MAX RVR       : TRUE\n");
         strcat(outstring, temp);
      }

      if( Mptr->RRVR[i].Max_visRange != MAXINT ) {
         sprintf(temp, "MX R_WAY VISRNG (%s): %d\n",
                 distance_as_string(Mptr->RRVR[i].distance_unit),
                 Mptr->RRVR[i].Max_visRange);
         strcat(outstring, temp);
      }

      if( Mptr->RRVR[i].Min_visRange != MAXINT ) {
         sprintf(temp, "MN R_WAY VISRNG (%s): %d\n",
                 distance_as_string(Mptr->RRVR[i].distance_unit),
                 Mptr->RRVR[i].Min_visRange);
         strcat(outstring, temp);
      }

   }


   if( Mptr->DVR.visRange != MAXINT ) {
      sprintf(temp, "DISPATCH VIS RANGE  : %d\n",
              Mptr->DVR.visRange);
      strcat(outstring, temp);
   }

   if ( Mptr->DVR.vrbl_visRange ) {
      sprintf(temp, "VRBL DISPATCH VISRNG: TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->DVR.below_min_DVR ) {
      sprintf(temp, "BELOW MIN DVR       : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->DVR.above_max_DVR ) {
      sprintf(temp, "ABOVE MAX DVR       : TRUE\n");
      strcat(outstring, temp);
   }

   if( Mptr->DVR.Max_visRange != MAXINT ) {
      sprintf(temp, "MX DSPAT VISRNG (FT): %d\n",
              Mptr->DVR.Max_visRange);
      strcat(outstring, temp);
   }

   if( Mptr->DVR.Min_visRange != MAXINT ) {
      sprintf(temp, "MN DSPAT VISRNG (FT): %d\n",
              Mptr->DVR.Min_visRange);
      strcat(outstring, temp);
   }

   for (i = 0; Mptr->WxObstruct[i][0] != '\0' && i < MAXWXSYMBOLS; i++)
   {
      sprintf(temp, "WX/OBSTRUCT VISION  : %s\n",
         Mptr->WxObstruct[i] );
      strcat(outstring, temp);
   }

   for (i = 0; i < MAX_PARTIAL_OBSCURATIONS; i++) {
       if ( Mptr->PartialObscurationAmt[i][0] != '\0' ) {
	  sprintf(temp, "OBSCURATION AMOUNT  : %s\n",
		Mptr->PartialObscurationAmt[i]);
	  strcat(outstring, temp);
       }

       if ( Mptr->PartialObscurationPhenom[i][0] != '\0' ) {
	  sprintf(temp, "OBSCURATION PHENOM  : %s\n",
		Mptr->PartialObscurationPhenom[i]);
	  strcat(outstring, temp);
       }
    }

   for (i = 0;  Mptr->cloudGroup[ i ].cloud_type[0] != '\0' &&
                     i < MAX_CLOUD_GROUPS; i++ )
   {
      if ( Mptr->cloudGroup[ i ].cloud_type[0] != '\0' ) {
         sprintf(temp, "CLOUD COVER         : %s\n",
            Mptr->cloudGroup[ i ].cloud_type);
         strcat(outstring, temp);
      }

      if ( Mptr->cloudGroup[ i ].cloud_hgt_char[0] != '\0' ) {
         sprintf(temp, "CLOUD HGT (CHARAC.) : %s\n",
            Mptr->cloudGroup[ i ].cloud_hgt_char);
         strcat(outstring, temp);
      }

      if ( Mptr->cloudGroup[ i ].cloud_hgt_meters != MAXINT) {
         sprintf(temp, "CLOUD HGT (METERS)  : %d\n",
            Mptr->cloudGroup[ i ].cloud_hgt_meters);
         strcat(outstring, temp);
      }

      if ( Mptr->cloudGroup[ i ].other_cld_phenom[0] != '\0' ) {
         sprintf(temp, "OTHER CLOUD PHENOM  : %s\n",
            Mptr->cloudGroup[ i ].other_cld_phenom);
         strcat(outstring, temp);
      }
   }

   if ( Mptr->temp != MAXINT ) {
      sprintf(temp, "TEMP. (CELSIUS)     : %d\n", Mptr->temp);
      strcat(outstring, temp);
   }

   if ( Mptr->dew_pt_temp != MAXINT ) {
      sprintf(temp, "D.P. TEMP. (CELSIUS): %d\n", Mptr->dew_pt_temp);
      strcat(outstring, temp);
   }

   if ( Mptr->A_altstng ) {
      sprintf(temp, "ALTIMETER (INCHES)  : %.2f\n",
         Mptr->inches_altstng );
      strcat(outstring, temp);
   }

   if ( Mptr->Q_altstng ) {
      sprintf(temp, "ALTIMETER (PASCALS) : %d\n",
         Mptr->hectoPasc_altstng );
      strcat(outstring, temp);
   }

    sprintf_tornadic_info (outstring, Mptr);

   if ( Mptr->autoIndicator[0] != '\0' ) {
         sprintf(temp, "AUTO INDICATOR      : %s\n",
                          Mptr->autoIndicator);
      strcat(outstring, temp);
   }

   if ( Mptr->PKWND_dir !=  MAXINT ) {
      sprintf(temp, "PEAK WIND DIRECTION : %d\n",Mptr->PKWND_dir);
      strcat(outstring, temp);
   }
   if ( Mptr->PKWND_speed !=  MAXINT ) {
      sprintf(temp, "PEAK WIND SPEED     : %d\n",Mptr->PKWND_speed);
      strcat(outstring, temp);
   }
   if ( Mptr->PKWND_hour !=  MAXINT ) {
      sprintf(temp, "PEAK WIND HOUR      : %d\n",Mptr->PKWND_hour);
      strcat(outstring, temp);
   }
   if ( Mptr->PKWND_minute !=  MAXINT ) {
      sprintf(temp, "PEAK WIND MINUTE    : %d\n",Mptr->PKWND_minute);
      strcat(outstring, temp);
   }

   if ( Mptr->WshfTime_hour != MAXINT ) {
      sprintf(temp, "HOUR OF WIND SHIFT  : %d\n",Mptr->WshfTime_hour);
      strcat(outstring, temp);
   }
   if ( Mptr->WshfTime_minute != MAXINT ) {
      sprintf(temp, "MINUTE OF WIND SHIFT: %d\n",Mptr->WshfTime_minute);
      strcat(outstring, temp);
   }
   if ( Mptr->Wshft_FROPA != FALSE ) {
      sprintf(temp, "FROPA ASSOC. W/WSHFT: TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->TWR_VSBY != (float) MAXINT ) {
      sprintf(temp, "TOWER VISIBILITY    : %.2f\n",Mptr->TWR_VSBY);
      strcat(outstring, temp);
   }
   if ( Mptr->SFC_VSBY != (float) MAXINT ) {
      sprintf(temp, "SURFACE VISIBILITY  : %.2f\n",Mptr->SFC_VSBY);
      strcat(outstring, temp);
   }

   if ( Mptr->minVsby != (float) MAXINT ) {
      sprintf(temp, "MIN VRBL_VIS (SM)   : %.4f\n",Mptr->minVsby);
      strcat(outstring, temp);
   }
   if ( Mptr->maxVsby != (float) MAXINT ) {
      sprintf(temp, "MAX VRBL_VIS (SM)   : %.4f\n",Mptr->maxVsby);
      strcat(outstring, temp);
   }

   if( Mptr->VSBY_2ndSite != (float) MAXINT ) {
      sprintf(temp, "VSBY_2ndSite (SM)   : %.4f\n",Mptr->VSBY_2ndSite);
      strcat(outstring, temp);
   }

   if( Mptr->VSBY_2ndSite_LOC[0] != '\0' ) {
      sprintf(temp, "VSBY_2ndSite LOC.   : %s\n",
                   Mptr->VSBY_2ndSite_LOC);
      strcat(outstring, temp);
   }

   if ( Mptr->OCNL_LTG ) {
      sprintf(temp, "OCCASSIONAL LTG     : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->FRQ_LTG ) {
      sprintf(temp, "FREQUENT LIGHTNING  : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->CNS_LTG ) {
      sprintf(temp, "CONTINUOUS LTG      : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->CG_LTG ) {
      sprintf(temp, "CLOUD-GROUND LTG    : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->IC_LTG ) {
      sprintf(temp, "IN-CLOUD LIGHTNING  : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->CC_LTG ) {
      sprintf(temp, "CLD-CLD LIGHTNING   : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->CA_LTG ) {
      sprintf(temp, "CLOUD-AIR LIGHTNING : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->AP_LTG ) {
      sprintf(temp, "LIGHTNING AT AIRPORT: TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->OVHD_LTG ) {
      sprintf(temp, "LIGHTNING OVERHEAD  : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->DSNT_LTG ) {
      sprintf(temp, "DISTANT LIGHTNING   : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->LightningVCTS ) {
      sprintf(temp, "L'NING W/I 5-10(ALP): TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->LightningTS ) {
      sprintf(temp, "L'NING W/I 5 (ALP)  : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->VcyStn_LTG ) {
      sprintf(temp, "VCY STN LIGHTNING   : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->LTG_DIR[0] != '\0' ) {
      sprintf(temp, "DIREC. OF LIGHTNING : %s\n", Mptr->LTG_DIR);
      strcat(outstring, temp);
   }



   i = 0;
   while( i < 3 && Mptr->ReWx[ i ].Recent_weather[0] != '\0' )
   {
      sprintf(temp, "RECENT WEATHER      : %s",
                  Mptr->ReWx[i].Recent_weather);
      strcat(outstring, temp);

      if ( Mptr->ReWx[i].Bhh != MAXINT ) {
         sprintf(temp, " BEG_hh = %d",Mptr->ReWx[i].Bhh);
         strcat(outstring, temp);
      }
      if ( Mptr->ReWx[i].Bmm != MAXINT ) {
         sprintf(temp, " BEG_mm = %d",Mptr->ReWx[i].Bmm);
         strcat(outstring, temp);
      }

      if ( Mptr->ReWx[i].Ehh != MAXINT ) {
         sprintf(temp, " END_hh = %d",Mptr->ReWx[i].Ehh);
         strcat(outstring, temp);
      }
      if ( Mptr->ReWx[i].Emm != MAXINT ) {
         sprintf(temp, " END_mm = %d",Mptr->ReWx[i].Emm);
         strcat(outstring, temp);
      }

      strcat(outstring, "\n");

      i++;
   }

   if ( Mptr->minCeiling != MAXINT ) {
      sprintf(temp, "MIN VRBL_CIG (FT)   : %d\n",Mptr->minCeiling);
      strcat(outstring, temp);
   }
   if ( Mptr->maxCeiling != MAXINT ) {
      sprintf(temp, "MAX VRBL_CIG (FT))  : %d\n",Mptr->maxCeiling);
      strcat(outstring, temp);
   }

   if ( Mptr->CIG_2ndSite_Meters != MAXINT ) {
      sprintf(temp, "CIG2ndSite (FT)     : %d\n",Mptr->CIG_2ndSite_Meters);
      strcat(outstring, temp);
   }
   if ( Mptr->CIG_2ndSite_LOC[0] != '\0' ) {
      sprintf(temp, "CIG @ 2nd Site LOC. : %s\n",Mptr->CIG_2ndSite_LOC);
      strcat(outstring, temp);
   }

   if ( Mptr->PRESFR ) {
      sprintf(temp, "PRESFR              : TRUE\n");
      strcat(outstring, temp);
   }
   if ( Mptr->PRESRR ) {
      sprintf(temp, "PRESRR              : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->SLPNO ) {
      sprintf(temp, "SLPNO               : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->SLP != (float) MAXINT ) {
      sprintf(temp, "SLP (hPa)           : %.1f\n", Mptr->SLP);
      strcat(outstring, temp);
   }

   if ( Mptr->SectorVsby != (float) MAXINT ) {
      sprintf(temp, "SECTOR VSBY (MILES) : %.2f\n", Mptr->SectorVsby );
      strcat(outstring, temp);
   }

   if ( Mptr->SectorVsby_Dir[ 0 ] != '\0' ) {
      sprintf(temp, "SECTOR VSBY OCTANT  : %s\n", Mptr->SectorVsby_Dir );
      strcat(outstring, temp);
   }

   if ( Mptr->TS_LOC[ 0 ] != '\0' ) {
      sprintf(temp, "THUNDERSTORM LOCAT. : %s\n", Mptr->TS_LOC );
      strcat(outstring, temp);
   }

   if ( Mptr->TS_MOVMNT[ 0 ] != '\0' ) {
      sprintf(temp, "THUNDERSTORM MOVMNT.: %s\n", Mptr->TS_MOVMNT);
      strcat(outstring, temp);
   }

   if ( Mptr->GR ) {
      sprintf(temp, "GR (HAILSTONES)     : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->GR_Size != (float) MAXINT ) {
      sprintf(temp, "HLSTO SIZE (INCHES) : %.3f\n",Mptr->GR_Size);
      strcat(outstring, temp);
   }

   if ( Mptr->VIRGA ) {
      sprintf(temp, "VIRGA               : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->VIRGA_DIR[0] != '\0' ) {
      sprintf(temp, "DIR OF VIRGA FRM STN: %s\n", Mptr->VIRGA_DIR);
      strcat(outstring, temp);
   }

   for( i = 0; i < MAX_SURFACE_OBSCURATIONS; i++ ) {
      if( Mptr->SfcObscuration[i][0] != '\0' ) {
         sprintf(temp, "SfcObscuration      : %s\n",
                   Mptr->SfcObscuration[i]);
         strcat(outstring, temp);
      }
   }

   if ( Mptr->Num8thsSkyObscured != MAXINT ) {
      sprintf(temp, "8ths of SkyObscured : %d\n",Mptr->Num8thsSkyObscured);
      strcat(outstring, temp);
   }

   if ( Mptr->CIGNO ) {
      sprintf(temp, "CIGNO               : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->Ceiling != MAXINT ) {
      sprintf(temp, "Ceiling (ft)        : %d\n",Mptr->Ceiling);
      strcat(outstring, temp);
   }

   if ( Mptr->Estimated_Ceiling != MAXINT ) {
      sprintf(temp, "Estimated CIG (ft)  : %d\n",Mptr->Estimated_Ceiling);
      strcat(outstring, temp);
   }

   if ( Mptr->VrbSkyBelow[0] != '\0' ) {
      sprintf(temp, "VRB SKY COND BELOW  : %s\n",Mptr->VrbSkyBelow);
      strcat(outstring, temp);
   }

   if ( Mptr->VrbSkyAbove[0] != '\0' ) {
      sprintf(temp, "VRB SKY COND ABOVE  : %s\n",Mptr->VrbSkyAbove);
      strcat(outstring, temp);
   }

   if ( Mptr->VrbSkyLayerHgt != MAXINT ) {
      sprintf(temp, "VRBSKY COND HGT (FT): %d\n",Mptr->VrbSkyLayerHgt);
      strcat(outstring, temp);
   }

   if ( Mptr->ObscurAloftHgt != MAXINT ) {
      sprintf(temp, "Hgt Obscur Aloft(ft): %d\n",Mptr->ObscurAloftHgt);
      strcat(outstring, temp);
   }

   if ( Mptr->ObscurAloft[0] != '\0' ) {
      sprintf(temp, "Obscur Phenom Aloft : %s\n",Mptr->ObscurAloft);
      strcat(outstring, temp);
   }

   if ( Mptr->ObscurAloftSkyCond[0] != '\0' ) {
      sprintf(temp, "Obscur ALOFT SKYCOND: %s\n",Mptr->ObscurAloftSkyCond);
      strcat(outstring, temp);
   }


   if ( Mptr->NOSPECI ) {
      sprintf(temp, "NOSPECI             : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->LAST ) {
      sprintf(temp, "LAST                : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->synoptic_cloud_type[ 0 ] != '\0' ) {
      sprintf(temp, "SYNOPTIC CLOUD GROUP: %s\n",Mptr->synoptic_cloud_type);
      strcat(outstring, temp);
   }

   if ( Mptr->CloudLow != '\0' ) {
      sprintf(temp, "LOW CLOUD CODE      : %c\n",Mptr->CloudLow);
      strcat(outstring, temp);
   }

   if ( Mptr->CloudMedium != '\0' ) {
      sprintf(temp, "MEDIUM CLOUD CODE   : %c\n",Mptr->CloudMedium);
      strcat(outstring, temp);
   }

   if ( Mptr->CloudHigh != '\0' ) {
      sprintf(temp, "HIGH CLOUD CODE     : %c\n",Mptr->CloudHigh);
      strcat(outstring, temp);
   }

   if ( Mptr->SNINCR != MAXINT ) {
      sprintf(temp, "SNINCR (INCHES)     : %d\n",Mptr->SNINCR);
      strcat(outstring, temp);
   }

   if ( Mptr->SNINCR_TotalDepth != MAXINT ) {
      sprintf(temp, "SNINCR(TOT. INCHES) : %d\n",Mptr->SNINCR_TotalDepth);
      strcat(outstring, temp);
   }

   if ( Mptr->snow_depth_group[ 0 ] != '\0' ) {
      sprintf(temp, "SNOW DEPTH GROUP    : %s\n",Mptr->snow_depth_group);
      strcat(outstring, temp);
   }

   if ( Mptr->snow_depth != MAXINT ) {
      sprintf(temp, "SNOW DEPTH (INCHES) : %d\n",Mptr->snow_depth);
      strcat(outstring, temp);
   }

   if ( Mptr->WaterEquivSnow != (float) MAXINT ) {
      sprintf(temp, "H2O EquivSno(inches): %.2f\n",Mptr->WaterEquivSnow);
      strcat(outstring, temp);
   }

   if ( Mptr->SunshineDur != MAXINT ) {
      sprintf(temp, "SUNSHINE (MINUTES)  : %d\n",Mptr->SunshineDur);
      strcat(outstring, temp);
   }

   if ( Mptr->SunSensorOut ) {
      sprintf(temp, "SUN SENSOR OUT      : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->hourlyPrecip != (float) MAXINT ) {
      sprintf(temp, "HRLY PRECIP (INCHES): %.2f\n",Mptr->hourlyPrecip);
      strcat(outstring, temp);
   }

   if( Mptr->precip_amt != (float) MAXINT) {
      sprintf(temp, "3/6HR PRCIP (INCHES): %.2f\n",
         Mptr->precip_amt);
      strcat(outstring, temp);
   }

   if( Mptr->Indeterminant3_6HrPrecip ) {
      sprintf(temp, "INDTRMN 3/6HR PRECIP: TRUE\n");
      strcat(outstring, temp);
   }

   if( Mptr->precip_24_amt !=  (float) MAXINT) {
      sprintf(temp, "24HR PRECIP (INCHES): %.2f\n",
         Mptr->precip_24_amt);
      strcat(outstring, temp);
   }

   if ( Mptr->Indeterminant_24HrPrecip ) {
      sprintf(temp, "INDTRMN 24 HR PRECIP: TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->Temp_2_tenths != (float) MAXINT ) {
      sprintf(temp, "TMP2TENTHS (CELSIUS): %.1f\n",Mptr->Temp_2_tenths);
      strcat(outstring, temp);
   }

   if ( Mptr->DP_Temp_2_tenths != (float) MAXINT ) {
      sprintf(temp, "DPT2TENTHS (CELSIUS): %.1f\n",Mptr->DP_Temp_2_tenths);
      strcat(outstring, temp);
   }

   if ( Mptr->maxtemp !=  (float) MAXINT) {
      sprintf(temp, "MAX TEMP (CELSIUS)  : %.1f\n",
         Mptr->maxtemp);
      strcat(outstring, temp);
   }

   if ( Mptr->mintemp !=  (float) MAXINT) {
      sprintf(temp, "MIN TEMP (CELSIUS)  : %.1f\n",
         Mptr->mintemp);
      strcat(outstring, temp);
   }

   if ( Mptr->max24temp !=  (float) MAXINT) {
      sprintf(temp, "24HrMAXTMP (CELSIUS): %.1f\n",
         Mptr->max24temp);
      strcat(outstring, temp);
   }

   if ( Mptr->min24temp !=  (float) MAXINT) {
      sprintf(temp, "24HrMINTMP (CELSIUS): %.1f\n",
         Mptr->min24temp);
      strcat(outstring, temp);
   }

   if ( Mptr->char_prestndcy != MAXINT) {
      sprintf(temp, "CHAR PRESS TENDENCY : %d\n",
         Mptr->char_prestndcy );
      strcat(outstring, temp);
   }

   if ( Mptr->prestndcy != (float) MAXINT) {
      sprintf(temp, "PRES. TENDENCY (hPa): %.1f\n",
         Mptr->prestndcy );
      strcat(outstring, temp);
   }

   if ( Mptr->PWINO ) {
      sprintf(temp, "PWINO               : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->PNO ) {
      sprintf(temp, "PNO                 : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->CHINO ) {
      sprintf(temp, "CHINO               : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->CHINO_LOC[0] != '\0' ) {
      sprintf(temp, "CHINO_LOC           : %s\n",Mptr->CHINO_LOC);
      strcat(outstring, temp);
   }

   if ( Mptr->VISNO ) {
      sprintf(temp, "VISNO               : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->VISNO_LOC[0] != '\0' ) {
      sprintf(temp, "VISNO_LOC           : %s\n",Mptr->VISNO_LOC);
      strcat(outstring, temp);
   }

   if ( Mptr->FZRANO ) {
      sprintf(temp, "FZRANO              : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->TSNO ) {
      sprintf(temp, "TSNO                : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->DollarSign) {
      sprintf(temp, "DOLLAR $IGN INDCATR : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->horiz_vsby[ 0 ] != '\0' ) {
      sprintf(temp, "HORIZ VISIBILITY    : %s\n",Mptr->horiz_vsby);
      strcat(outstring, temp);
   }

   if ( Mptr->dir_min_horiz_vsby[ 0 ] != '\0' ) {
      sprintf(temp, "DIR MIN HORIZ VSBY  : %s\n",Mptr->dir_min_horiz_vsby);
      strcat(outstring, temp);
   }

   if ( Mptr->CAVOK ) {
      sprintf(temp, "CAVOK               : TRUE\n");
      strcat(outstring, temp);
   }


   if( Mptr->VertVsby != MAXINT ) {
      sprintf(temp, "Vert. Vsby (meters) : %d\n",
                  Mptr->VertVsby );
      strcat(outstring, temp);
   }

   if ( Mptr->QFE != MAXINT ) {
      sprintf(temp, "QFE                 : %d\n", Mptr->QFE);
      strcat(outstring, temp);
   }

   if ( Mptr->VOLCASH ) {
      sprintf(temp, "VOLCANIC ASH        : TRUE\n");
      strcat(outstring, temp);
   }

   if ( Mptr->min_vrbl_wind_dir != MAXINT ) {
      sprintf(temp, "MIN VRBL WIND DIR   : %d\n",Mptr->min_vrbl_wind_dir);
      strcat(outstring, temp);
   }
   if ( Mptr->max_vrbl_wind_dir != MAXINT ) {
      sprintf(temp, "MAX VRBL WIND DIR   : %d\n",Mptr->max_vrbl_wind_dir);
      strcat(outstring, temp);
   }


   strcat(outstring, "\n\n\n");
}


void print_decoded_metar (Decoded_METAR *Mptr)
{
	char stringbuf[5000];

	sprint_metar(stringbuf, Mptr);
	fputs(stringbuf, stdout);
}

// vim: set ts=4 sw=4 sts=4 noet :
