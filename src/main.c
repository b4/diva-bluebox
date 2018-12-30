/*-----------------------------------------------------------------------------
 *
 * Copyright 2001-2010    Dialogic(R) Corporation
 *
 * All Rights Reserved
 *
 * This software is the property of Dialogic Corporation and/or its
 * subsidiaries ("Dialogic"). This copyright notice may not be removed,
 * modified or obliterated without the prior written permission of
 * Dialogic.
 *
 * No right, title, ownership or other interest in the software is hereby
 * granted or transferred. The information contained herein is subject 
 * to change without notice and should not be construed as a commitment of
 * Dialogic.
 *
 * This application code is not part of any standard Dialogic product and is
 * provided to you solely for the purpose of assisting you in the development
 * of your applications with Dialogic Diva SDK. The code is provided "AS IS",
 * without warranty of any kind. Dialogic shall not be liable for any 
 * damages arising out of your use of this application, even if it has been
 * advised of the possibility of such damages
 *
 *-----------------------------------------------------------------------------
 * Sample for answering or making a single call and send DTMF digits and display 
 * received DTMF digits.  If running on IP based devices the mode for sending
 * and receiving DTMF can be selected.
 *
 * Note that this sample is designed to show very simple how process a 
 * single call and to stream and record audio. Therefore the sample does
 * not do any error handling.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#ifdef WIN32
    #include <conio.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "dssdk.h"



/*
 * Some globals
 */
DivaAppHandle   hApp = NULL;
DivaCallHandle  hSdkCall = NULL;
AppCallHandle   hMyCall = NULL;
DWORD           DtmfMode = DivaDtmfModeAuto;
char            CallingNumber[100] = { 0 };

void CallbackHandler ( DivaAppHandle hApp, DivaEvent Event, PVOID Param1, PVOID Param2 )
{
	switch (Event)
	{
	case DivaEventIncomingCall:
        printf ( "Incoming call\n" );
        if ( hSdkCall == NULL )
        {
            hSdkCall = Param1;
            DivaSetCallProperties ( hSdkCall, DivaCPT_DTMFMode, (DivaCallPropertyValue*) & DtmfMode, sizeof ( DWORD ) );
            DivaAnswer ( hSdkCall, hMyCall, DivaCallTypeVoice );
        }
		break;

	case DivaEventCallConnected:
        printf ( "Call connected\n" );
        DivaReportDTMF ( hSdkCall,  TRUE );
        DivaReportTones ( hSdkCall, FALSE );
        break;

    case DivaEventSendVoiceEnded:
		break;

    case DivaEventRecordVoiceEnded:
		break;

    case DivaEventDTMFReceived:
        printf ( "DTMF char %c received\n", (char) (long) Param2 );
        break;

    case DivaEventSendDTMFToneEnded:
        printf ( "DTMF tone sent\n" );
        break;

	case DivaEventCallDisconnected:
        if ( Param2 == hSdkCall )
        {
            hSdkCall = 0;
            printf ( "Call disconnected\n" );
        }
		DivaCloseCall ( Param2 );
		break;

    default:
        break;
    }
}

void    Usage ()
{
    printf ( "Use:  VoiceSetSIPHeader -d <device> -n<Dialstring> [-c<calling number>]\n" );
}

int main(int argc, char* argv[])
{
    int     c;
    int     i;
    char    DtmfString[2];
    DWORD   Result;
    char    DialString[100];
    DWORD   Device;


    DialString[0] = 0;
    Device = LINEDEV_ALL;

	printf("Version: %d.%d\n", (DivaGetVersion() >> 16) & 0xffff , DivaGetVersion() & 0xffff );

    for ( i = 1 ; i < argc ; i++ )
    {
        if ( *argv[i] == '-' )
        {
            switch ( *(argv[i] + 1) )
            {
            case 'n':
                strcpy ( DialString, argv[i] + 2 );
                break;

            case 'd':
                Device = atoi ( argv[i] + 2  );
                break;

            case 'c':
                strcpy ( CallingNumber, argv[i] + 2 );
                break;
            default:
                Usage ( );
                return 0;
            }
        }
        else
        {
            Usage ();
            return 0;
        }
    }
    

	if (DivaInitialize() != DivaSuccess)
		return -1;

    hMyCall = (void *) 0x11223344;

    if ( DivaRegister ( &hApp, DivaEventModeCallback, (void *) CallbackHandler, 0, 0, 0, 1024 ) != DivaSuccess )
    {
        return -1;
    }

    if ( DialString[0] )
    {
        if ( DivaConnectVoice ( hApp, hMyCall, &hSdkCall, DialString, Device, CallingNumber, "", 0 ) != DivaSuccess )
            return -1;
    }
    else
    {
        if ( DivaListen ( hApp, DivaListenAll, Device, "" ) != DivaSuccess )
            return -1;
    }

    printf ( "%s\n", DialString[0] == 0 ? "Waiting for call" : "Dialing" );
    printf ( "----------------------------------------------------\n" );
    printf ( "Press q           to terminate\n" );
    printf ( "Press 0 .. 9,     to send single MF \n" );
    printf ( "Press s           to send 2600hz\n" );
    printf ( "Press k	          to send KP\n" );
    printf ( "Press t	          to send ST\n" );
    printf ( "Press f             to send 2100hz\n" );

    while ( 1 )
    {

#ifdef WIN32
        c = _getch ( );
#else
        c = getc (stdin);
#endif
        if ( c == 'q' )
            break;

        switch ( c )
        {
        case 'c':
            break;

        case '0':
            if ( hSdkCall != NULL )
      	    {
                Result = DivaGenerateDualTone ( hSdkCall, 1300, 14000, 1500, 14000, 55 );
               	printf ( "Tone generation result: %d\n", Result );
	   }
	    break;	    
        case '1':
            if ( hSdkCall != NULL )
            {
               	Result = DivaGenerateDualTone ( hSdkCall, 700, 14000, 900, 14000, 55 );
               	printf ( "Tone generation result: %d\n", Result );
     	   }
            break;
        case '2':
            if ( hSdkCall != NULL )
            {
             	Result = DivaGenerateDualTone ( hSdkCall, 700, 14000, 1100, 14000, 55 );
            	printf ( "Tone generation result: %d\n", Result );
           }
            break;
        case '3':
            if ( hSdkCall != NULL )
            {
             	Result = DivaGenerateDualTone ( hSdkCall, 900, 14000, 1100, 14000, 55 );
            	printf ( "Tone generation result: %d\n", Result );
           }
            break;
        case '4':
            if ( hSdkCall != NULL )
            {
             	Result = DivaGenerateDualTone ( hSdkCall, 700, 14000, 1300, 14000, 55 );
            	printf ( "Tone generation result: %d\n", Result );
           }
            break;
        case '5':
            if ( hSdkCall != NULL )
            {
             	Result = DivaGenerateDualTone ( hSdkCall, 900, 14000, 1300, 14000, 55 );
            	printf ( "Tone generation result: %d\n", Result );
           }
            break;
        case '6':
            if ( hSdkCall != NULL )
            {
             	Result = DivaGenerateDualTone ( hSdkCall, 1100, 14000, 1300, 14000, 55 );
            	printf ( "Tone generation result: %d\n", Result );
           }
            break;
        case '7':
            if ( hSdkCall != NULL )
            {
             	Result = DivaGenerateDualTone ( hSdkCall, 700, 14000, 1500, 14000, 55 );
            	printf ( "Tone generation result: %d\n", Result );
           }
            break;
        case '8':
            if ( hSdkCall != NULL )
            {
             	Result = DivaGenerateDualTone ( hSdkCall, 900, 14000, 1500, 14000, 55 );
            	printf ( "Tone generation result: %d\n", Result );
           }
            break;
        case '9':
            if ( hSdkCall != NULL )
            {
             	Result = DivaGenerateDualTone ( hSdkCall, 1100, 14000, 1500, 14000, 55 );
            	printf ( "Tone generation result: %d\n", Result );
           }
            break;
        case 'k':
            if ( hSdkCall != NULL )
            {
             	Result = DivaGenerateDualTone ( hSdkCall, 1100, 14000, 1700, 14000, 110 );
            	printf ( "Tone generation result: %d\n", Result );
           }
            break;
        case 't':
            if ( hSdkCall != NULL )
            {
             	Result = DivaGenerateDualTone ( hSdkCall, 1500, 14000, 1700, 14000, 110 );
            	printf ( "Tone generation result: %d\n", Result );
           }
            break;
        case 's':
            if ( hSdkCall != NULL )
            {
		Result = DivaGenerateSingleTone ( hSdkCall, 2600, 14000, 500 );
		printf ( "Tone generation result: %d\n", Result );
            }
            break;
        case 'f':
            if ( hSdkCall != NULL )
            {
               	Result = DivaGenerateSingleTone ( hSdkCall, 2100, 14000, 2200 );
               	printf ( "Tone generation result: %d\n", Result );
            }
            break;
	}	

    }
    printf ( "Tests stopped\n" );

    DivaUnregister ( hApp );
    DivaTerminate ();

    return ( 0 );
};
