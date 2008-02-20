/* Classic Ladder Project */
/* Copyright (C) 2001-2008 Marc Le Douarain */
/* http://membres.lycos.fr/mavati/classicladder/ */
/* http://www.sourceforge.net/projects/classicladder */
/* July 2003 */
/* ----------------------------- */
/* Editor Config - GTK interface */
/* ----------------------------- */
/* This library is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU Lesser General Public */
/* License as published by the Free Software Foundation; either */
/* version 2.1 of the License, or (at your option) any later version. */

/* This library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU */
/* Lesser General Public License for more details. */

/* You should have received a copy of the GNU Lesser General Public */
/* License along with this library; if not, write to the Free Software */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

// modified for EMC 
// Chris Morley Feb 08

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "classicladder.h"
#include "manager.h"
#include "edit.h"
//#include "hardware.h"
#include "global.h"
#include "config_gtk.h"

#ifdef OLD_TIMERS_MONOS_SUPPORT
#define NBR_OBJECTS 15
#else
#define NBR_OBJECTS 13
#endif
GtkWidget *LabelParam[ NBR_OBJECTS ],*ValueParam[ NBR_OBJECTS ];
#ifndef HAL_SUPPORT
static char * Devices[] = { "None", "DirectPortAccess",
#ifdef COMEDI_SUPPORT
"/dev/comedi0", "/dev/comedi1", "/dev/comedi2", "/dev/comedi3",
#endif
 NULL };
#endif

#define NBR_IO_PARAMS 6
GtkWidget *InputParamEntry[ NBR_INPUTS_CONF ][ NBR_IO_PARAMS ];
GtkWidget *InputDeviceParam[ NBR_INPUTS_CONF ];
GtkWidget *InputFlagParam[ NBR_INPUTS_CONF ];

GtkWidget *OutputParamEntry[ NBR_OUTPUTS_CONF ][ NBR_IO_PARAMS ];
GtkWidget *OutputDeviceParam[ NBR_OUTPUTS_CONF ];
GtkWidget *OutputFlagParam[ NBR_OUTPUTS_CONF ];

#ifdef MODBUS_IO_MASTER
static char * ModbusReqType[] = { "Inputs", "Coils", /* TODO: "WriteRegs", "ReadRegs",*/ NULL };
#define NBR_MODBUS_PARAMS 6
GtkWidget *ModbusParamEntry[ NBR_MODBUS_MASTER_REQ ][ NBR_MODBUS_PARAMS ];
GtkWidget *SerialPortEntry;
GtkWidget *SerialSpeedEntry;
GtkWidget *PauseInterFrameEntry;
GtkWidget *DebugLevelEntry;
#endif

GtkWidget *ConfigWindow;

GtkWidget * CreateGeneralParametersPage( void )
{
	GtkWidget *vbox;
	GtkWidget *hbox[ NBR_OBJECTS ];
	int NumObj;

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);

	for (NumObj=0; NumObj<NBR_OBJECTS; NumObj++)
	{
		char BuffLabel[ 50 ];
		char BuffValue[ 20 ];
		int InfoUsed = 0;
		hbox[NumObj] = gtk_hbox_new (FALSE, 0);
		gtk_container_add (GTK_CONTAINER (vbox), hbox[NumObj]);
		gtk_widget_show (hbox[NumObj]);

		switch( NumObj )
		{
			case 0:
				sprintf( BuffLabel, "Periodic Refresh Rate (ms)" );
				sprintf( BuffValue, "%d", InfosGene->GeneralParams.PeriodicRefreshMilliSecs );
				break;
			case 1:
				InfoUsed = GetNbrRungsDefined( )*100/InfosGene->GeneralParams.SizesInfos.nbr_rungs;
				sprintf( BuffLabel, "Nbr.rungs (%d%c used - current alloc=%d)", InfoUsed,'%', InfosGene->GeneralParams.SizesInfos.nbr_rungs );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_rungs );
				break;
			case 2:
				sprintf( BuffLabel, "Nbr.Bits (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_bits );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_bits );
				break;
			case 3:
				sprintf( BuffLabel, "Nbr.Words (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_words );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_words );
				break;
			case 4:
				sprintf( BuffLabel, "Nbr.Counters (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_counters );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_counters );
				break;
			case 5:
				sprintf( BuffLabel, "Nbr.Timers IEC (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_timers_iec );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_timers_iec );
				break;
			case 6:
				sprintf( BuffLabel, "Nbr.Phys.Inputs (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_phys_inputs );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_phys_inputs );
				break;
			case 7:
				sprintf( BuffLabel, "Nbr.Phys.Outputs (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_phys_outputs );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_phys_outputs );
				break;
			case 8:
				sprintf( BuffLabel, "Nbr.Arithm.Expr. (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_arithm_expr );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_arithm_expr );
				break;
			case 9:
				InfoUsed = NbrSectionsDefined( )*100/InfosGene->GeneralParams.SizesInfos.nbr_sections;
				sprintf( BuffLabel, "Nbr.Sections (%d%c used - current alloc=%d)", InfoUsed,'%', InfosGene->GeneralParams.SizesInfos.nbr_sections );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_sections );
				break;
			case 10:
				sprintf( BuffLabel, "Nbr.Symbols (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_symbols );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_symbols );
				break;
#ifdef OLD_TIMERS_MONOS_SUPPORT
			case 11:
				sprintf( BuffLabel, "Nbr.Timers (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_timers );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_timers );
				break;
			case 12:
				sprintf( BuffLabel, "Nbr.Monostables (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_monostables );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_monostables );
				break;
#endif
			case 13:
				sprintf( BuffLabel, "Nbr.S32in (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_s32in );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_s32in );
				break;
			case 14:
				sprintf( BuffLabel, "Nbr.S32out (current alloc=%d)", InfosGene->GeneralParams.SizesInfos.nbr_s32out );
				sprintf( BuffValue, "%d", GeneralParamsMirror.SizesInfos.nbr_s32out );
				break;
			default:
				sprintf( BuffLabel, "???" );
				sprintf( BuffValue, "???" );
				break;
		}

		LabelParam[NumObj] = gtk_label_new(BuffLabel);
		gtk_widget_set_usize((GtkWidget *)LabelParam[NumObj],300,0);
		gtk_box_pack_start (GTK_BOX (hbox[NumObj]), LabelParam[NumObj], FALSE, FALSE, 0);
		gtk_widget_show (LabelParam[NumObj]);

		/* For numbers */
		ValueParam[NumObj] = gtk_entry_new();
		gtk_widget_set_usize((GtkWidget *)ValueParam[NumObj],50,0);
		gtk_box_pack_start (GTK_BOX (hbox[NumObj]), ValueParam[NumObj], FALSE, FALSE, 0);
		gtk_widget_show (ValueParam[NumObj]);
		gtk_entry_set_text( GTK_ENTRY(ValueParam[NumObj]), BuffValue);

//		gtk_editable_set_editable( GTK_EDITABLE(ValueParam[NumObj]), (NumObj==0)?TRUE:FALSE);
	}
	return vbox;
}
int GetOneGeneralInfo( int iNumber )
{
	char text[ 10 ];
	int value;
	strncpy( text, (char *)gtk_entry_get_text((GtkEntry *)ValueParam[ iNumber ]), 10 );
	text[ 9 ] = '\0';
	value = atoi( text );
	return value;
}
void GetGeneralParameters( void )
{
	int TheValue;
	TheValue = GetOneGeneralInfo( 0 );
	if ( TheValue<1 || TheValue>1000 )
		TheValue = PERIODIC_REFRESH_MS_DEF;
	InfosGene->GeneralParams.PeriodicRefreshMilliSecs = TheValue;
	GeneralParamsMirror.PeriodicRefreshMilliSecs = TheValue;

	TheValue = GetOneGeneralInfo( 1 );
	GeneralParamsMirror.SizesInfos.nbr_rungs = TheValue;
	TheValue = GetOneGeneralInfo( 2 );
	GeneralParamsMirror.SizesInfos.nbr_bits = TheValue;
	TheValue = GetOneGeneralInfo( 3 );
	GeneralParamsMirror.SizesInfos.nbr_words = TheValue;
	TheValue = GetOneGeneralInfo( 4 );
	GeneralParamsMirror.SizesInfos.nbr_counters = TheValue;
	TheValue = GetOneGeneralInfo( 5 );
	GeneralParamsMirror.SizesInfos.nbr_timers_iec = TheValue;
	TheValue = GetOneGeneralInfo( 6 );
	GeneralParamsMirror.SizesInfos.nbr_phys_inputs = TheValue;
	TheValue = GetOneGeneralInfo( 7 );
	GeneralParamsMirror.SizesInfos.nbr_phys_outputs = TheValue;
	TheValue = GetOneGeneralInfo( 8 );
	GeneralParamsMirror.SizesInfos.nbr_arithm_expr = TheValue;
	TheValue = GetOneGeneralInfo( 9 );
	GeneralParamsMirror.SizesInfos.nbr_sections = TheValue;
	TheValue = GetOneGeneralInfo( 10 );
	GeneralParamsMirror.SizesInfos.nbr_symbols = TheValue;
#ifdef OLD_TIMERS_MONOS_SUPPORT
	TheValue = GetOneGeneralInfo( 11 );
	GeneralParamsMirror.SizesInfos.nbr_timers = TheValue;
	TheValue = GetOneGeneralInfo( 12 );
	GeneralParamsMirror.SizesInfos.nbr_monostables = TheValue;
#endif
	TheValue = GetOneGeneralInfo( 13 );
	GeneralParamsMirror.SizesInfos.nbr_s32in = TheValue;
	TheValue = GetOneGeneralInfo( 14 );
 	GeneralParamsMirror.SizesInfos.nbr_s32out = TheValue;
}
#ifndef HAL_SUPPORT
GtkWidget * CreateIOConfPage( char ForInputs )
{
	static char * Labels[] = { "First %", "Type", "PortAdr/SubDev", "First Channel", "Nbr Channels", "Logic" };
	GtkWidget *vbox;
	GtkWidget *hbox[ (ForInputs?NBR_INPUTS_CONF:NBR_OUTPUTS_CONF)+1   +30];
	int NumObj;
	int NumLine;
	GList * ItemsDevices = NULL;
	int ScanDev = 0;
	StrIOConf * pConf;
	GtkWidget *InputParamLabel[ NBR_IO_PARAMS];
	GtkWidget *OutputParamLabel[ NBR_IO_PARAMS ];
int testpack;

	do
	{
		ItemsDevices = g_list_append( ItemsDevices, Devices[ ScanDev++ ] );
	}
	while( Devices[ ScanDev ] );

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);

	for (NumLine=-1; NumLine<(ForInputs?NBR_INPUTS_CONF:NBR_OUTPUTS_CONF); NumLine++ )
	{
		hbox[NumLine+1] = gtk_hbox_new (FALSE, 0);
		gtk_container_add (GTK_CONTAINER (vbox), hbox[NumLine+1]);
		
		gtk_widget_show (hbox[NumLine+1]);
		for (NumObj=0; NumObj<NBR_IO_PARAMS; NumObj++)
		{
			if ( NumLine==-1 )
			{
				GtkWidget **IOParamLabel = ForInputs?&InputParamLabel[ NumObj ]:&OutputParamLabel[ NumObj ];
				if ( NumObj==0 )
					*IOParamLabel = gtk_label_new( ForInputs?"1st %I mapped":"1st %Q mapped" );
				else
					*IOParamLabel = gtk_label_new( Labels[ NumObj ] );
				gtk_widget_set_usize(*IOParamLabel,NumObj==1?130:90,0);
				gtk_box_pack_start(GTK_BOX (hbox[ NumLine+1 ]), *IOParamLabel, FALSE, FALSE, 0);
				gtk_widget_show( *IOParamLabel );
			}
			else
			{
				char BuffValue[ 30 ];
				if ( ForInputs )
					pConf = &InfosGene->InputsConf[ NumLine ];
				else
					pConf = &InfosGene->OutputsConf[ NumLine ];

				switch( NumObj )
				{
					/* For devices */
					case 1:
					{
						int ValueToDisplay = pConf->DeviceType;
						if ( pConf->FirstClassicLadderIO==-1 )
						{
							ValueToDisplay = 0;
						}
						else
						{
							if ( ValueToDisplay==DEVICE_TYPE_DIRECT_ACCESS )
								ValueToDisplay = 1;
							if ( ValueToDisplay>=DEVICE_TYPE_COMEDI )
								ValueToDisplay = ValueToDisplay-DEVICE_TYPE_COMEDI+2;
						}
						{
							GtkWidget **IOParamDevice = ForInputs?&InputDeviceParam[ NumLine ]:&OutputDeviceParam[ NumLine ];
							*IOParamDevice = gtk_combo_new( );
							gtk_combo_set_value_in_list( GTK_COMBO(*IOParamDevice), TRUE /*val*/, FALSE /*ok_if_empty*/ );
							gtk_combo_set_popdown_strings( GTK_COMBO(*IOParamDevice), ItemsDevices );
							gtk_widget_set_usize( *IOParamDevice,130,0 );
							gtk_box_pack_start ( GTK_BOX (hbox[NumLine+1]), *IOParamDevice, FALSE, FALSE, 0 );
							gtk_widget_show ( *IOParamDevice );
					        	gtk_entry_set_text((GtkEntry*)((GtkCombo *)*IOParamDevice)->entry,Devices[ ValueToDisplay ]);
						}
						break;
					}
					/* For flags */
					case 5:
					{
						GtkWidget **IOParamFlag = ForInputs?&InputFlagParam[ NumLine ]:&OutputFlagParam[ NumLine ];
						*IOParamFlag = gtk_check_button_new_with_label( "Inverted" );
						gtk_widget_set_usize( *IOParamFlag,90,0 );
						gtk_box_pack_start( GTK_BOX (hbox[NumLine+1]), *IOParamFlag, FALSE, FALSE, 0 );
						gtk_widget_show ( *IOParamFlag );
						if ( pConf->FlagInverted )
							gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( *IOParamFlag ), TRUE );
						break;
					}
					/* For numbers */
					default:
					{
						switch( NumObj )
						{
							case 0:
								if ( pConf->FirstClassicLadderIO==-1 )
									strcpy( BuffValue, "" );
								else
									sprintf( BuffValue, "%d", pConf->FirstClassicLadderIO );
								break;
							case 2:
								if ( pConf->DeviceType==DEVICE_TYPE_DIRECT_ACCESS )
									sprintf( BuffValue, "%X", pConf->SubDevOrAdr );
								else
									sprintf( BuffValue, "%d", pConf->SubDevOrAdr );
								break;
							case 3:
								sprintf( BuffValue, "%d", pConf->FirstChannel ); break;
							case 4:
								sprintf( BuffValue, "%d", pConf->NbrConsecutivesChannels ); break;
						}
						{
							GtkWidget **IOParamEntry = ForInputs?&InputParamEntry[ NumLine ][ NumObj ]:&OutputParamEntry[ NumLine ][ NumObj ];
							*IOParamEntry = gtk_entry_new( );
							gtk_widget_set_usize( *IOParamEntry,90,0 );
							gtk_box_pack_start( GTK_BOX (hbox[NumLine+1]), *IOParamEntry, FALSE, FALSE, 0 );
							gtk_widget_show ( *IOParamEntry );
							gtk_entry_set_text( GTK_ENTRY(*IOParamEntry), BuffValue );
						}
						break;
					}
				}
			}
		}//for (NumObj=0;
	}

//TODO: I've not found how to not have all the hbox vertically expanded...?
for(testpack=0; testpack<30; testpack++)
{		
		hbox[(ForInputs?NBR_INPUTS_CONF:NBR_OUTPUTS_CONF)+1+testpack] = gtk_hbox_new (FALSE, 0);
		gtk_container_add (GTK_CONTAINER (vbox), hbox[(ForInputs?NBR_INPUTS_CONF:NBR_OUTPUTS_CONF)+1+testpack]);
//gtk_box_pack_start(GTK_BOX(vbox), hbox[ (ForInputs?NBR_INPUTS_CONF:NBR_OUTPUTS_CONF)+1+testpack ], TRUE, TRUE, 0);
		gtk_widget_show (hbox[(ForInputs?NBR_INPUTS_CONF:NBR_OUTPUTS_CONF)+1+testpack]);
}

	
	return vbox;
}
#endif
int ConvComboToNum( char * text, char ** list )
{
	int Value = 0;
	char Found = FALSE;
	while( !Found && list[ Value ]!=NULL )
	{
		if ( strcmp( list[ Value ], text )==0 )
			Found = TRUE;
		else
			Value++;
	}
	return Value;
}

// we don't want this function if using EMC
#ifndef HAL_SUPPORT 
void GetIOSettings( char ForInputs )
{
	int NumObj;
	int NumLine;
	StrIOConf * pConf;
	int ComboVal;
	GtkWidget **IOParamDevice;
	GtkWidget **IOParamEntry;
	GtkWidget **IOParamFlag;
	char * text;
	for (NumLine=0; NumLine<(ForInputs?NBR_INPUTS_CONF:NBR_OUTPUTS_CONF); NumLine++ )
	{
		if ( ForInputs )
			pConf = &InfosGene->InputsConf[ NumLine ];
		else
			pConf = &InfosGene->OutputsConf[ NumLine ];

		pConf->FirstClassicLadderIO = -1;
		pConf->FlagInverted = 0;

		IOParamDevice = ForInputs?&InputDeviceParam[ NumLine ]:&OutputDeviceParam[ NumLine ];
		ComboVal = ConvComboToNum( (char *)gtk_entry_get_text((GtkEntry *)((GtkCombo *)*IOParamDevice)->entry), Devices );
		if ( ComboVal>0 )
		{
			int FirstIO = -1;
			if ( ComboVal==1 )
				pConf->DeviceType = DEVICE_TYPE_DIRECT_ACCESS;
			else
				pConf->DeviceType = DEVICE_TYPE_COMEDI+ComboVal-2;
			IOParamFlag = ForInputs?&InputFlagParam[ NumLine ]:&OutputFlagParam[ NumLine ];
			if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON( *IOParamFlag ) ) )
				pConf->FlagInverted = 1;
			for (NumObj=0; NumObj<NBR_IO_PARAMS; NumObj++)
			{
				IOParamEntry = ForInputs?&InputParamEntry[ NumLine ][ NumObj ]:&OutputParamEntry[ NumLine ][ NumObj ];
				switch( NumObj )
				{
					case 0:
						text = (char *)gtk_entry_get_text((GtkEntry *)*IOParamEntry);
						FirstIO = atoi( text );
						break;
					case 2:
						text = (char *)gtk_entry_get_text((GtkEntry *)*IOParamEntry);
						if ( pConf->DeviceType==DEVICE_TYPE_DIRECT_ACCESS )
							sscanf( text, "%X", &pConf->SubDevOrAdr );
						else
							pConf->SubDevOrAdr = atoi( text );
						break;
					case 3:
						text = (char *)gtk_entry_get_text((GtkEntry *)*IOParamEntry);
						pConf->FirstChannel = atoi( text );
						break;
					case 4:
						text = (char *)gtk_entry_get_text((GtkEntry *)*IOParamEntry);
						pConf->NbrConsecutivesChannels = atoi( text );
						break;
				}
			}
			/* verify if not overflowing */
			if ( FirstIO+pConf->NbrConsecutivesChannels>( ForInputs?NBR_PHYS_INPUTS:NBR_PHYS_OUTPUTS ) )
			{
				printf("Error in I/O conf: overflow for Ixx or Qxx mapping detected...\n" );
				FirstIO = -1;
			}
			/* done at the end, do not forget multi-task ! */
			pConf->FirstClassicLadderIO = FirstIO;
		}//if ( ComboVal>0 )
	}
}
#endif
#ifdef MODBUS_IO_MASTER
GtkWidget * CreateModbusModulesIO( void )
{
	static char * Labels[] = { "Slave Address", "TypeAccess", "1st Modbus Ele.", "Nbr Modbus Ele.", "Logic", "1st %I/%Q Mapped" };
	GtkWidget *vbox;
	GtkWidget *hbox[ NBR_MODBUS_MASTER_REQ+2 ];
	int NumObj;
	int NumLine;
	GList * ItemsDevices = NULL;
	int ScanDev = 0;
	StrModbusMasterReq * pConf;
	char BuffValue[ 40 ];
	GtkWidget *ModbusParamLabel[ NBR_MODBUS_PARAMS];	
	GtkWidget *SerialPortLabel;
	GtkWidget *SerialSpeedLabel;
	GtkWidget *PauseInterFrameLabel;
	GtkWidget *DebugLevelLabel;

	do
	{
		ItemsDevices = g_list_append( ItemsDevices, ModbusReqType[ ScanDev++ ] );
	}
	while( ModbusReqType[ ScanDev ] );

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);

	for (NumLine=-2; NumLine<NBR_MODBUS_MASTER_REQ; NumLine++ )
	{
		hbox[NumLine+2] = gtk_hbox_new (FALSE, 0);
		gtk_container_add (GTK_CONTAINER (vbox), hbox[NumLine+2]);
		gtk_widget_show (hbox[NumLine+2]);

		for (NumObj=0; NumObj<NBR_MODBUS_PARAMS; NumObj++)
		{
			switch( NumLine )
			{
				case -2:
				{
					if ( NumObj==0 )
					{
						SerialPortLabel = gtk_label_new( "Serial port (blank = IP mode)" );
						gtk_box_pack_start(GTK_BOX (hbox[ NumLine+2 ]), SerialPortLabel, FALSE, FALSE, 0);
						gtk_widget_show( SerialPortLabel );
						SerialPortEntry = gtk_entry_new( );
						gtk_box_pack_start( GTK_BOX (hbox[NumLine+2]), SerialPortEntry, FALSE, FALSE, 0 );
						gtk_widget_show ( SerialPortEntry );
						gtk_entry_set_text( GTK_ENTRY(SerialPortEntry), ModbusSerialPortNameUsed );
//TODO: configplc file written by hand for now...
gtk_editable_set_editable( GTK_EDITABLE(SerialPortEntry), FALSE);
						
						SerialSpeedLabel = gtk_label_new( "Serial speed" );
						gtk_box_pack_start(GTK_BOX (hbox[ NumLine+2 ]), SerialSpeedLabel, FALSE, FALSE, 0);
						gtk_widget_show( SerialSpeedLabel );
						SerialSpeedEntry = gtk_entry_new( );
						gtk_widget_set_usize( SerialSpeedEntry,90,0 );
						gtk_box_pack_start( GTK_BOX (hbox[NumLine+2]), SerialSpeedEntry, FALSE, FALSE, 0 );
						gtk_widget_show ( SerialSpeedEntry );
						sprintf( BuffValue, "%d", ModbusSerialSpeed );
						gtk_entry_set_text( GTK_ENTRY(SerialSpeedEntry), BuffValue );
//TODO: configplc file written by hand for now...
gtk_editable_set_editable( GTK_EDITABLE(SerialSpeedEntry), FALSE);
						
						PauseInterFrameLabel = gtk_label_new( "Pause Inter-Frame" );
						gtk_box_pack_start(GTK_BOX (hbox[ NumLine+2 ]), PauseInterFrameLabel, FALSE, FALSE, 0);
						gtk_widget_show( PauseInterFrameLabel );
						PauseInterFrameEntry = gtk_entry_new( );
						gtk_widget_set_usize( PauseInterFrameEntry,90,0 );
						gtk_box_pack_start( GTK_BOX (hbox[NumLine+2]), PauseInterFrameEntry, FALSE, FALSE, 0 );
						gtk_widget_show ( PauseInterFrameEntry );
						sprintf( BuffValue, "%d", ModbusTimeInterFrame );
						gtk_entry_set_text( GTK_ENTRY(PauseInterFrameEntry), BuffValue );
//TODO: configplc file written by hand for now...
gtk_editable_set_editable( GTK_EDITABLE(PauseInterFrameEntry), FALSE);
						
						DebugLevelLabel = gtk_label_new( "Debug level" );
						gtk_box_pack_start(GTK_BOX (hbox[ NumLine+2 ]), DebugLevelLabel, FALSE, FALSE, 0);
						gtk_widget_show( DebugLevelLabel );
						DebugLevelEntry = gtk_entry_new( );
						gtk_widget_set_usize( DebugLevelEntry,70,0 );
						gtk_box_pack_start( GTK_BOX (hbox[NumLine+2]), DebugLevelEntry, FALSE, FALSE, 0 );
						gtk_widget_show ( DebugLevelEntry );
						sprintf( BuffValue, "%d", ModbusDebugLevel );
						gtk_entry_set_text( GTK_ENTRY(DebugLevelEntry), BuffValue );
					}
					break;
				}
				case -1:
				{
					GtkWidget **IOParamLabel = &ModbusParamLabel[ NumObj ];
					*IOParamLabel = gtk_label_new( Labels[ NumObj ] );
					gtk_widget_set_usize(*IOParamLabel,(NumObj<=1 || NumObj==5)?120:100,0);
					gtk_box_pack_start(GTK_BOX (hbox[ NumLine+2 ]), *IOParamLabel, FALSE, FALSE, 0);
					gtk_widget_show( *IOParamLabel );
					break;
				}
				default:
				{
					pConf = &ModbusMasterReq[ NumLine ];
					switch( NumObj )
					{
						/* For req type (combo-list) */
						case 1:
						{
							int ValueToDisplay = pConf->TypeReq;
							GtkWidget **IOParamDevice = &ModbusParamEntry[ NumLine ][ NumObj ];
							*IOParamDevice = gtk_combo_new( );
							gtk_combo_set_value_in_list( GTK_COMBO(*IOParamDevice), TRUE /*val*/, FALSE /*ok_if_empty*/ );
							gtk_combo_set_popdown_strings( GTK_COMBO(*IOParamDevice), ItemsDevices );
							gtk_widget_set_usize( *IOParamDevice,120,0 );
							gtk_box_pack_start ( GTK_BOX (hbox[NumLine+2]), *IOParamDevice, FALSE, FALSE, 0 );
							gtk_widget_show ( *IOParamDevice );
					        	gtk_entry_set_text((GtkEntry*)((GtkCombo *)*IOParamDevice)->entry, ModbusReqType[ ValueToDisplay ]);
							break;
						}
						/* For flags */
						case 4:
						{
							GtkWidget **IOParamFlag = &ModbusParamEntry[ NumLine ][ NumObj ];
							*IOParamFlag = gtk_check_button_new_with_label( "Inverted" );
							gtk_widget_set_usize( *IOParamFlag,100,0 );
							gtk_box_pack_start( GTK_BOX (hbox[NumLine+2]), *IOParamFlag, FALSE, FALSE, 0 );
							gtk_widget_show ( *IOParamFlag );
							if ( pConf->LogicInverted )
								gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( *IOParamFlag ), TRUE );
							break;
						}
						/* For numbers/strings */
						default:
						{
							switch( NumObj )
							{
								case 0:
									strcpy( BuffValue, pConf->SlaveAdr );
									break;
								case 2:
									sprintf( BuffValue, "%d", pConf->FirstModbusElement );
									break;
								case 3:
									sprintf( BuffValue, "%d", pConf->NbrModbusElements );
									break;
								case 5:
									sprintf( BuffValue, "%d", pConf->OffsetVarMapped );
									break;
							}
							{
								GtkWidget **IOParamEntry = &ModbusParamEntry[ NumLine ][ NumObj ];
								*IOParamEntry = gtk_entry_new( );
								gtk_widget_set_usize( *IOParamEntry,(NumObj<=1 || NumObj==5)?120:100,0 );
								gtk_box_pack_start( GTK_BOX (hbox[NumLine+2]), *IOParamEntry, FALSE, FALSE, 0 );
								gtk_widget_show ( *IOParamEntry );
								gtk_entry_set_text( GTK_ENTRY(*IOParamEntry), BuffValue );
							}
							break;
						}
					}
				}//default:
			}
		}
	}
	return vbox;
}
void GetModbusModulesIOSettings( void )
{
	int NumObj;
	int NumLine;
	StrModbusMasterReq * pConf;
	GtkWidget **IOParamEntry;
	char * text;
	char BuffValue[ 40 ];
	for (NumLine=0; NumLine<NBR_MODBUS_MASTER_REQ; NumLine++ )
	{
		pConf = &ModbusMasterReq[ NumLine ];
		strcpy( pConf->SlaveAdr, "" );
		pConf->LogicInverted = 0;

		for (NumObj=0; NumObj<NBR_IO_PARAMS; NumObj++)
		{
			IOParamEntry = &ModbusParamEntry[ NumLine ][ NumObj ];
			switch( NumObj )
			{
				case 0:
					text = (char *)gtk_entry_get_text((GtkEntry *)*IOParamEntry);
					strcpy( BuffValue, text );
					break;
				case 1:
					pConf->TypeReq = ConvComboToNum( (char *)gtk_entry_get_text((GtkEntry *)((GtkCombo *)*IOParamEntry)->entry), ModbusReqType );
					break;
				case 2:
					text = (char *)gtk_entry_get_text((GtkEntry *)*IOParamEntry);
					pConf->FirstModbusElement = atoi( text );
					break;
				case 3:
					text = (char *)gtk_entry_get_text((GtkEntry *)*IOParamEntry);
					pConf->NbrModbusElements = atoi( text );
					break;
				case 4:
					if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON( *IOParamEntry ) ) )
						pConf->LogicInverted = 1;
					break;
				case 5:
					text = (char *)gtk_entry_get_text((GtkEntry *)*IOParamEntry);
					pConf->OffsetVarMapped = atoi( text );
					break;
			}
		}//for (NumObj=0; 
		/* verify if not overflowing */
		if ( pConf->OffsetVarMapped+pConf->NbrModbusElements>( (pConf->TypeReq==MODBUS_REQ_INPUTS_READ)?NBR_PHYS_INPUTS:NBR_PHYS_OUTPUTS ) )
		{
			printf("Error in I/O modbus conf: overflow for Ixx or Qxx mapping detected...\n" );
			strcpy( BuffValue, "" );
		}
		/* done at the end, do not forget multi-task ! */
		/* the first char is tested to determine a valid request => paranoia mode ;-) */
		strcpy( &pConf->SlaveAdr[ 1 ], &BuffValue[ 1 ] );
		pConf->SlaveAdr[ 0 ] = BuffValue[ 0 ];
	}//for (NumLine=0; 

	text = (char *)gtk_entry_get_text(GTK_ENTRY(SerialPortEntry));
	strcpy( ModbusSerialPortNameUsed ,text );
	text = (char *)gtk_entry_get_text(GTK_ENTRY(SerialSpeedEntry));
	ModbusSerialSpeed = atoi( text );
	text = (char *)gtk_entry_get_text(GTK_ENTRY(PauseInterFrameEntry));
	ModbusTimeInterFrame = atoi( text );
	text = (char *)gtk_entry_get_text(GTK_ENTRY(DebugLevelEntry));
	ModbusDebugLevel = atoi( text );
}
#endif

// we dont want to change general parameters from user program -only when loading realtime
// we also dont want calls to get IO settings in EMC (we use HAL instead)
// configHardware is not used in EMC
void GetSettings( void )
{
	//GetGeneralParameters( );
#ifndef HAL_SUPPORT
	GetIOSettings( 1/*ForInputs*/ );
	GetIOSettings( 0/*ForInputs*/ );
#endif
#ifdef MODBUS_IO_MASTER
	GetModbusModulesIOSettings( );
#endif
#ifndef RT_SUPPORT
//	ConfigHardware( );
	InfosGene->AskToConfHard = TRUE;
#endif
}

// we don't want to show physical input/ouput if using EMC in this function
void OpenConfigWindowGtk()
{
	GtkWidget *nbook;

	ConfigWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title( GTK_WINDOW(ConfigWindow), "Config" );
	gtk_window_set_modal( GTK_WINDOW(ConfigWindow), TRUE );

	nbook = gtk_notebook_new( );
	gtk_notebook_append_page( GTK_NOTEBOOK(nbook), CreateGeneralParametersPage( ),
				 gtk_label_new ("Period/Sizes") );
#ifndef HAL_SUPPORT 
	gtk_notebook_append_page( GTK_NOTEBOOK(nbook), CreateIOConfPage( 1/*ForInputs*/ ),
				 gtk_label_new ("Physical Inputs") );
	gtk_notebook_append_page( GTK_NOTEBOOK(nbook), CreateIOConfPage( 0/*ForInputs*/ ),
				 gtk_label_new ("Physical Outputs") );
#endif 
#ifdef MODBUS_IO_MASTER
	gtk_notebook_append_page( GTK_NOTEBOOK(nbook), CreateModbusModulesIO( ),
				 gtk_label_new ("Modbus distributed I/O") );
#endif

	gtk_container_add( GTK_CONTAINER (ConfigWindow), nbook );
	gtk_widget_show( nbook );

	gtk_window_set_position( GTK_WINDOW(ConfigWindow), GTK_WIN_POS_CENTER );
//	gtk_window_set_policy( GTK_WINDOW(ConfigWindow), FALSE, FALSE, TRUE );
	gtk_signal_connect ( GTK_OBJECT(ConfigWindow), "destroy",
                        GTK_SIGNAL_FUNC(GetSettings), NULL );

	gtk_widget_show( ConfigWindow );
}


