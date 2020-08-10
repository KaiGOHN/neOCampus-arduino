/* -----------------------------------------------------------------------------
 *
 * neOCampus operation
 * 
 * neOSensor based on ESP32 boards
 * -----------------------------------------------------------------------------
 * 
 * NOTES:
 * - you need to 'deploy' our boards definitions
 * - select 'neOSensor AirQuality' board from menu (located end of list)
 * - NO MORE NEED TO PATCH standard libs ... we now have our own defines !!! :)
 * - 'NEOSENSOR_BOARD' compilation flag tells which bord it is (i.e NEOSENSOR_AIRQUALITY)
 * -----------------------------------------------------------------------------
 * 
 * TODO:
 * - everything!
 * -----------------------------------------------------------------------------
 * 
 * F.DeMiguel
 * F.Thiebolt   aug.20  initial release
 * ----------------------------------------------------------------------------- */



/* #############################################################################
 * ###                                                                       ###
 * ###                       BOARD HARDWARE SELECTION                        ###
 * ###                                                                       ###
 * 
 *      No more need to modify the proper board in >>> 'neocampus.h' <<<
 *      because we now use a compilation flag -DNEOSENSOR_BOARD :)
 * 
 * >>>     Don't forget to update BOARD_FWREV from your 'board/neOXxx.h'     <<<
 * 
 * ############################################################################# */


/*
 * Includes
 */
/* specify timezone; daylight will automatically follow :) */
#include <TZ.h>

#include <Arduino.h>
#include <Wire.h>
#include <Ticker.h>

/* As of esp8266 arduino lib >=2.4.0, time is managed via local or sntp along with TZ support :) */ 
#include <time.h>                       // time() ctime()
#include <coredecls.h>                  // settimeofday_cb(), tune_timeshift64()

#include <sntp.h>
//#include <lwipopts.h>                   // for SNTP_UPDATE_DELAY (1 hour default, it's ok :) )

/*
 * ESP8266 advanced ops
 */
#if defined ESP8266
  extern "C" {
    #include "user_interface.h"           // wifi_station_dhcpc_start, ...
  }
  extern "C" {
    #include "espconn.h"                  // espconn_tcp_set_max_con() set maximum number of simultaneous TCP connexions
  }
#endif /* ESP8266 */



/* neOCampus related includes */
#include "neocampus.h"

#include "neocampus_debug.h"
#include "neocampus_utils.h"
#include "neocampus_i2c.h"
#include "sensocampus.h"
#include "neocampus_eeprom.h"
#include "neocampus_OTA.h"




#if 0
// neOCampus modules
#include "device.h"
#include "temperature.h"
#include "humidity.h"
#include "luminosity.h"
#include "noise.h"
#include "neoclock.h"

// modules management
#include "modulesMgt.h"

// WiFi parameters management
#include "wifiParametersMgt.h"
#endif /* 0 */


/*
 * Definitions
*/
// Debug related definitions
#define SERIAL_BAUDRATE   115200
#if (LOG_LEVEL > 0) || defined(DEBUG_WIFI_MANAGER)
  #include <SoftwareSerial.h>
#endif

// sensOCampus related definitions
#define SENSO_MAX_RETRIES         5   // maximum number of sensOCampus configuration retrieval retries

// Time related definitions
#define MYTZ                      TZ_Europe_Paris
#define NTP_DEFAULT_SERVER1       "0.fr.pool.ntp.org" // DNS location aware
//#define NTP_DEFAULT_SERVER2       "time.nist.gov"     // DNS location aware
#define NTP_DEFAULT_SERVER2       "pool.ntp.org"      // DNS location aware
#define NTP_DEFAULT_SERVER3       "ntp.univ-tlse3.fr" // UT3 specific


/*
 * various led mods to express status of neOSensor
 * device while in setup()
 */
typedef enum {
  WIFI,
  DISABLE
} enum_ledmode_t;


/*
 * Global variables
 */
bool _need2reboot = false;              // flag to tell a reboot is requested


#if 0
// WiFi parameters management statically allocated instance
wifiParametersMgt wifiParameters = wifiParametersMgt();

// modules management statically allocated instance
modulesMgt modulesList = modulesMgt();

// sensOCampus statically allocated instance with link to global wifi parameters
senso sensocampus = senso( &wifiParameters );

// device statically allocated instance
device deviceModule = device();

// temperature class module
temperature *temperatureModule      = NULL;

// humidity class module
humidity *humidityModule            = NULL;

// luminosity class module
luminosity *luminosityModule        = NULL;

// noise class module
noise *noiseModule                  = NULL;
// noise module ISR :(
void ICACHE_RAM_ATTR noiseDetectISR() {             /* https://community.particle.io/t/cpp-attachinterrupt-to-class-function-help-solved/5147/2 */
  if( noiseModule ) noiseModule->noiseDetectISR();
}

// neoclock class module
neoclock *clockModule               = NULL;
#endif /* 0 */

// time server related
bool cbtime_set = false;
bool _cbtime_call = false;          // used to display things about time sync
time_t cbtime_cur, cbtime_prev;     // time set in callback






// --- Functions ---------------------------------------------------------------
void setupSerial( void ) {
#ifdef SERIAL_BAUDRATE
  delay(3000);  // time for USB serial link to come up anew
  Serial.begin(SERIAL_BAUDRATE); // Start serial for output

  // Arduino libs v2.4.1, to enable printf and debug messages output
  Serial.setDebugOutput( true );
  
  char tmp[64];
  snprintf(tmp,sizeof(tmp),"\n# %s firmware rev %d for neOCampus is starting ... ",getBoardName(),getFirmwareRev());
  log_info(tmp);
  log_info(F("\n#\tMac address is ")); log_info(getMacAddress());
  log_info(F("\n#\tlog level is ")); log_info(LOG_LEVEL,DEC);
  log_info(F("\n"));
  log_flush();
#endif
#ifndef DEBUG_WIFI_MANAGER
  wifiManager.setDebugOutput(false);
#endif
}


// ---
// Setup Serial link1 used to either core debugging or just to blink led2
void n( void ) {
  // ESP8266's GPIO2 is tied to Serial1's TX
  Serial1.begin( 115200, (SerialConfig)SERIAL_8N1, (SerialMode)UART_TX_ONLY, 2);

  // Arduino libs v2.4.1, to enable printf and debug messages output
  // Serial1.setDebugOutput( true ); // default
}


// ---
// clear Sensors WiFi, EEPROM, config files (SPIFFS) ...
// ... called upon clear SWITCH button activated at startup
void clearSensor( void ) {
  log_debug(F("\n[CLEAR] starting CLEAR procedure ..."));

  // clear WiFi credentials
  WiFi.disconnect();

  // invalidate EEPROM
  clearEEPROMvalidity();

  // clear JSON config files (SPIFFS)
  formatSPIFFS();
}


// ---
// I2C setup pins and frequency
void setupI2C( void ) {

  if( _need2reboot ) return;
  
  log_info(F("\n[i2c] start setup ..."));
  Wire.begin(SDA,SCL);
  #ifdef I2C_FREQ
    log_debug(F("\nI2C frequency set to "));log_debug(I2C_FREQ/1000,DEC); log_debug("kHz"); log_flush();
    Wire.setClock(I2C_FREQ);
  #else
    log_debug(F("\nI2C frequency set to default 100kHz")); log_flush();
  #endif
}


// ---
// process end of main loop: specific functions executed every seconds
void endLoop( void ) {
  static unsigned long _lastCheck = 0;    // elapsed ms since last check

  // check if a reboot has been requested ...
  if( _need2reboot ) {
    log_info(F("\n[REBOOT] a reboot has been asked ..."));log_flush();
    modulesList.stopAll();
    neOSensor_reboot();
    delay(5000); // to avoid an infinite loop
  }

  // a second elapsed ?
  if( ((millis() - _lastCheck) >= (unsigned long)1000UL) == true ) {
    _lastCheck = millis();

    /* ESP8266's onboard blue led (GPIO2) is tied to Serial1:
     * send zero(s) to blink the led
     */
    byte _msg[] = {0x00, 0x00, 0x00, 0x00 };
    Serial1.write(_msg, sizeof(_msg));Serial1.flush();

    // check Heap available
    if( ESP.getFreeHeap() < 4096 ) {
      log_error(F("\n[SYS] CRTICAL free heap very low!!!")); log_flush();
    }

    // display NTP servers used for time sync
    if( _cbtime_call ) {
      _cbtime_call = false;
      
      // list active ntp servers
      for( uint8_t i=0; i<SNTP_MAX_SERVERS; i++ ) {
        IPAddress sntp = *sntp_getserver(i);
        const char* sntp_name = sntp_getservername(i);
        if (sntp.isSet()) {
          log_debugF("\n[NTP][%d]:     ", i);
          if( sntp_name ) {
            log_debugF("%s (%s) ", sntp_name, sntp.toString().c_str());
          } else {
            log_debugF("%s ", sntp.toString().c_str());
          }
        }
      }      
    }

    // serial link activity marker ...
    log_debug(F("."));
  }
}


// --- LED MODES while in setup()
// ledWiFiMode
void _ledWiFiMode( uint8_t led ) {
  const uint8_t _val_steps = 50;
  static int16_t _val=0;
  
  if( _val < PWMRANGE ) _val+=_val_steps;
  else _val=-PWMRANGE;

  analogWrite( led, (abs(_val)<PWMRANGE) ? abs(_val) : PWMRANGE );
}


// various led modes to express neOSensor status while in setup()
void setupLed( uint8_t led, enum_ledmode_t led_mode ) {
  static Ticker timer_led;

  switch( led_mode ) {
    case WIFI:
      // set led to show we're waiting for WiFi connect
      pinMode(led,OUTPUT);
      timer_led.attach_ms(50, _ledWiFiMode, led);
      break;

    case DISABLE:
      // switch back to INPUT mode (default after reset)
      timer_led.detach();
      analogWrite( led, 0 );  // [Dec.17] workaround for resilient analog value written to led
      pinMode(led,INPUT);
      break;

    default:
      log_error(F("\n[setupLed] unknwown led_mode ?!?!"));
  }

  return;
}


// ---
/* sync ntp callback:
 *  This function gets called whenever a NTP sync occurs
 */
void syncNTP_cb( void ) {
  char _tmpbuf[64];
  struct tm *_tm;

  /* WARNING
   *  - gettimeofday --> TZ settings does not apply
   *  - time() while incorrect will get masked by localtime behaviour :s
   */
   
  // retrieve when time sync occured
  // gettimeofday( &cbtime_cur, NULL ); // WARNING: timezone does not apply ... :(
  // log_debug(F("\n[NTP] time sync occured ")); log_debug( ctime(&cbtime_cur.tv_sec) );
  time( &cbtime_cur );
  _tm = localtime( &cbtime_cur );   // Weird part ... localtime function *corrects* time to match timezone ... :s

  
  log_info(F("\n[NTP] sync CALLBACK ... ")); log_flush();

  /* Note: while in this callback, the ntp server
   *  is not yet written in the list of NTP servers!
  */

  // display time synchro message
  strftime( _tmpbuf, sizeof(_tmpbuf), "%Y-%m-%d %H:%M:%S %z", _tm);
  log_debug(F("\n[NTP] time sync occured ")); log_debug(_tmpbuf);

  // first call ?
  if( not cbtime_set ) {
    cbtime_set = true;
  }
  else {
    int16_t _sync_diff = cbtime_cur - ( cbtime_prev + (SNTP_UPDATE_DELAY/1000) ); // seconds
    if( abs(_sync_diff) <= 2 ) {
      log_info(F(" local clock well syncronized :)"));
    }
    else if( abs(_sync_diff) <= 60 ) {
      log_warning(F(" local clock shift(seconds)= "));log_info(_sync_diff,DEC);
    }
    else {
      log_debug(F(" local clock shift > 60s ... probably a WiFi resync ..."));
    }
  }
  log_flush();

  cbtime_prev = cbtime_cur;

  // callback called
  _cbtime_call = true;
}


// ---
// NTP setup
bool setupNTP( void ) {
  
  if( _need2reboot ) return false;

  log_debug(F("\n[NTP] start setup of (S)NTP ..."));

  // register ntp sync callback
  settimeofday_cb( syncNTP_cb );

  // [may.20] as default, NTP server is provided by the DHCP server
  configTime( MYTZ, NTP_DEFAULT_SERVER1, NTP_DEFAULT_SERVER2, NTP_DEFAULT_SERVER3 );

  log_flush();
  // the end ...
  return true;
}


// ---
// process WiFi Parameters options
void processWIFIparameters( wifiParametersMgt *wp=nullptr ) {
  
  if( !wp ) return;

  log_debug(F("\n[wifiParams] start to process options ... ")); log_flush();

  //
  // check for ERASE ALL OPTION
  if( wp->isEnabledEraseALL() ) {
    log_info(F("\n[wifiParams] a WHOLE ERASE has been requested ...")); log_flush();
    clearSensor();
    log_debug(F("\n\t... and asking for reboot ..."));
    delay(250);
    _need2reboot = true;  // set flag for reboot
    // and no need to go further ...
    return;
  }

  //
  // TM1637 display
  if( wp->isEnabled7segTM1637() and !clockModule ) {
    log_info(F("\n[wifiParams] start 7 segments neoclock (TM1637 driven) ...")); log_flush();
    
    // allocate display with TM1637 7 segment driver
    clockModule = new neoclock();
    clockModule->add_display( NEOCLOCK_TM1637 );


    // TODO: add local callback for getting luminosity, temperature, presence
    
  }
  else if( !wp->isEnabled7segTM1637() and clockModule ) {
    log_info(F("\n[wifiParams] removing 7 segments neoclock (TM1637 driven) ...")); log_flush();

    // stop and de-allocate object
    clockModule->stop();
    free( clockModule );
    clockModule = nullptr;
  }

  //
  // PIR sensor
  if( wp->isEnabledPIR() ) {
    log_error(F("\n[wifiParams] PIR sensor is not yet available ... stay tuned ;)")); log_flush();



    // TODO: activate PIR sensor


  }

  /*
   * Add processing for additional options here !
   */
}


// ---
// earlySetup: called at the very begining of setup()
void earlySetup( void ) {

  // note: no log_xxx msg since they will get defined later
  // log_info(F("\n[Early] disable autoConnect and clear saved IP ...")); log_flush();

#ifdef MAX_TCP_CONNECTIONS
  // set maximum number of simultaneous TCP connexions
  // log_info(F("\n[Early] set max TCP concurrent sockets to ")); log_info(MAX_TCP_CONNECTIONS, DEC); log_flush();
  espconn_tcp_set_max_con( MAX_TCP_CONNECTIONS );
#endif /* MAX_TCP_CONNECTIONS */

  // WiFi.disconnect(true); // to erase default credentials
  WiFi.setAutoConnect(false);

  // Disable WiFi sleep mode
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
}


// ---
// lateSetup: called at the very end of setup()
void lateSetup( void ) {

  if( _need2reboot ) {
    log_warning(F("\nA reboot has been asked while in setup() ... probably a faulty WiFi connect ...")); log_flush();
    delay(3000);
    modulesList.stopAll();
    neOSensor_reboot();
    delay(5000); // to avoid an infinite loop
  }
  
  char tmp[64];
  snprintf(tmp,sizeof(tmp),"\n#\n# %s firmware rev %d for neOCampus started :)",getBoardName(),getFirmwareRev());
  log_info(tmp);

  if( not cbtime_set ) {
    log_warning(F("\n#\t TIME NOT (YET) SYNCED !!"));
  }
  else {
    log_info(F("\n# current time is ")); log_info(getCurTime());
  }

  // for the (future) loop mode ...
  WiFi.setAutoConnect(true);

#ifdef MAX_TCP_CONNECTIONS
  log_info(F("\n# max TCP concurrent sockets = ")); log_info(MAX_TCP_CONNECTIONS, DEC); log_flush();
#endif /* MAX_TCP_CONNECTIONS */

  log_info(F("\n# --- --- ---")); log_flush();
}


// --- SETUP -------------------------------------------------------------------
void setup() {

  /*
   * Very first steps of setup :)
   */
  earlySetup();

  // obtain and store MAC address ...
  getMacAddress();

  /*
   * Serial link for debug ...
   */
  setupSerial();

  /*
   * Setup Serial1 to enable on-board blue led to blink
   */
  setupSerial1();

  /*
   * Check for CLEAR switch status:
   * if micro-switch active at startup for a specified delay, then clear all configurations.
   */
#ifdef CLEAR_SW
  if( checkCLEARswitch(CLEAR_SW) == true ) {
    // CLEAR sensor parameters (WiFi, modules, EEPROM etc)
    clearSensor();
  }
#endif

  /*
   * Setup neOCampus EEPROM
   */
  setupEEPROM();


  /*
   * Setup SPIFFS
   */
  setupSPIFFS();


  /* 
   * Led blinking for specific WiFi setup mode
   * - either with NOISE_LED (neOSensor)
   * - either with 7 segments (neOClock option)
   */
  wifiParameters.loadConfigFile();
  processWIFIparameters( &wifiParameters );
  if( clockModule ) {
    clockModule->animate();
  }
  else {
#ifdef NOISE_LED
    setupLed( NOISE_LED, (enum_ledmode_t)WIFI );
#endif
  }

  /*
   * setupNTP
   * Configure Timezone & DST
   * note: real ntp server may get sent from dhcp server :)
   */
  setupNTP();

  /*
   * WiFiManager to activate the network :)
   * - we added a 'sensOCampus' check box
   */
  setupWiFi( &wifiParameters );


  /*
   * Disable led blinking for WiFI setup mode since we're already connected
   */
  if( clockModule ) {
    clockModule->animate( false );
  }
  else {
#ifdef NOISE_LED
    setupLed( NOISE_LED, (enum_ledmode_t)DISABLE );
#endif
  }

  
  /*
   * Allocate time for ESP's firmware ...
   */
  yield();


  /*
   * Check for updates ...
   * - following example read a file containing config SSID and psk, it then compare to actual WiFi.SSID() and WiFi.psk()
   * to check if it needed to rerun a setup
   * https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/examples/OTA-mDNS-SPIFFS/OTA-mDNS-SPIFFS.ino
   * - ArduinoOTA callback may disable interrupts
   */
  neOCampusOTA();


  /*
   * Allocate time for ESP's firmware ...
   */
  yield();



  /*
   * SENSOCAMPUS client starts to retrieve
   * [jun.18] WiFiParameters passed within constructor
   * - MQTT server url
   * - MQTT server port
   * - MQTT username
   * - MQTT password (first time only)
   */
  uint8_t _retry=SENSO_MAX_RETRIES;
  while( not _need2reboot and sensocampus.begin(getMacAddress()) != true ) {
    log_info(F("\n[senso] WARNING unable to achieve sensOCampus credentials and/or config :|"));
    if( _retry-- ) {
      log_debug(F("\n\t... sleeping a bit before retrying ..."));
      delay((SENSO_MAX_RETRIES-_retry)*10*1000);
    }
    else {
      log_error(F("\n[senso] ERROR no more retries with sensOCampus ... reboot"));
      _need2reboot = true;
    }
  }
  // EEPROM dump (debug purpose ;)
  // hexdumpEEPROM();


  /*
   * Allocate time for ESP's firmware ...
   */
  yield();



  /*
   * Manage WiFi parameters options
   */
  processWIFIparameters( &wifiParameters );
  delay(250);  // to avoid race condition between I2C clock and TM1637 CLOCK (shared pin)


  /*
   * Allocate time for ESP's firmware ...
   */
  yield();



  /*
   * I2C bus initialization, set pins & frequency
   */
  setupI2C();


  /*
   * Allocate neOCampus sensors/actuators Modules
   */
  if( not _need2reboot ) {
    temperatureModule   = new temperature();
    humidityModule      = new humidity();
    luminosityModule    = new luminosity();
    #ifndef NOISE_DETECT
      noiseModule       = null; // means no noise sensor
    #elif NOISE_LED    // with or without led signaling noise
      noiseModule       = new noise( NOISE_DETECT, &noiseDetectISR, NOISE_LED );
    #else
      noiseModule       = new noise( NOISE_DETECT, &noiseDetectISR );
    #endif
    // add additional modules initialization here
  }

  /* 
   *  neOCampus modules instanciation
   *  - scan i2c bus
   *  - instantite components :)
   */
  uint8_t i2c_addr,res;
  
  i2c_addr=I2C_ADDR_START;
  log_debug(F("\nStart I2C scanning ..."));

  /* i2c scanner loop ...
   * [may.20] since some sensors are both kinds (e.g SHTXX --> temperature AND hygro)
   * ==> each i2c addr ought ot get tested against all kinds of modules 
   */
  do {
    if( _need2reboot ) break;
    
    res = i2c_scan(i2c_addr);
    if( res==uint8_t(-1) ) break;
    log_debug(F("\n\t... detected device at i2c_addr=0x"));log_debug(res,HEX);log_flush();

    bool _known = false;

    // is chip a temperature sensor ?
    if( temperatureModule and temperatureModule->add_sensor(res) == true ) {
      log_debug(F("\n\t\tadded temperature sensor at i2c addr = 0x"));log_debug(res,HEX); log_flush();
      _known = true;
    }
    // is chip a luminosity sensor ?
    if( luminosityModule and luminosityModule->add_sensor(res) == true ) {
      log_debug(F("\n\t\tadded luminosity sensor at i2c addr = 0x"));log_debug(res,HEX); log_flush();
      _known = true;
    }
    // is chip a DAC (part of a noise detection subsystem) ?
    if( noiseModule and noiseModule->add_dac(res) == true ) {
      log_debug(F("\n\t\tadded DAC to noise module whose i2c addr = 0x"));log_debug(res,HEX); log_flush();      
      _known = true;
    }
    // is chip a humidity sensor ?
    if( humidityModule and humidityModule->add_sensor(res) == true ) {
      log_debug(F("\n\t\tadded humidity sensor at i2c addr = 0x"));log_debug(res,HEX); log_flush();
      _known = true;
    }
    
    // add test for others modules ...

    // did the i2c deice has been identified ?
    if( not _known ) {
      log_warning(F("\n[WARNING] unknwown i2c device with i2c addr = 0x"));log_debug(res,HEX); log_flush();
    }
    
    // next iteration
    i2c_addr=res+1;

  } while( not _need2reboot and i2c_addr<=I2C_ADDR_STOP );

  // end of scanning
  log_debug(F("\n... END OF I2C scan ... "));log_flush();


  // add device module
  if( not modulesList.add(&deviceModule) ) {
    log_error(F("\n# unable to add deviceModule to the list ?!?! something weird is on way ... reboot asked"));log_flush();
    delay(5000);
    _need2reboot = true;
  }

  // check devices count in temperature module
  if( temperatureModule ) {
    if( temperatureModule->is_empty()==true or not modulesList.add(temperatureModule) ) {
      log_debug(F("\n# either temperature module does not have any sensor or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(temperatureModule);
      temperatureModule = NULL;
    }
  }
#ifdef TEMPERATURE_CORRECTION_LASTI2C
  if( temperatureModule ) {
    log_warning(F("\n\n[temperature] >>>WARNING<<< measures correction of last i2c sensor activated!\n")); log_flush();
    delay(1000);
  }
#endif
    

  // check devices count in humidity module
  if( humidityModule ) {
    if( humidityModule->is_empty()==true or not modulesList.add(humidityModule) ) {
      log_debug(F("\n# either humidity module does not have any sensor or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(humidityModule);
      humidityModule = NULL;
    }
  }

  
  // check devices count in luminosity module
  if( luminosityModule ) {
    if( luminosityModule->is_empty()==true or not modulesList.add(luminosityModule) ) {
      log_debug(F("\n# either luminosity module does not have any sensor or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(luminosityModule);
      luminosityModule = NULL;
    }
  }


  // check if noise module is ok
  if( noiseModule ) {
    if( noiseModule->is_empty()==true or not modulesList.add(noiseModule) ) {
      log_debug(F("\n# either noise module didn't detected any dac or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(noiseModule);
      noiseModule = NULL;
    }
  }


  // check if neoclock module is ok
  if( clockModule ) {
    if( clockModule->is_empty()==true or not modulesList.add(clockModule) ) {
      log_debug(F("\n# either neoclock module is empty or we've not been able to add it to the list of modules ... removing instance ..."));log_flush();
      free(clockModule);
      clockModule = NULL;
    }
  }


  // add check for additional neOCampus sensors/actuators modules


  /*
   * Allocate time for ESP's firmware ...
   */
  yield();


  /*
   * start all modules ...
   */
  modulesList.startAll( &sensocampus );


  /*
   * Very end of setup() :)
   */
  lateSetup();
}


// --- LOOP --------------------------------------------------------------------
void loop() {

  /*
   * Process device, module sensors ...
   */
  modulesList.processAll();

  /* 
   * end of main loop
   */
  // call endLoop system management level
  endLoop();
  
  // waiting a bit
  delay(250);
}