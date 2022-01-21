#include "events/mbed_events.h"
#include <mbed.h>

#include "SX1276_LoRaRadio.h"
#include "lorawan/LoRaWANInterface.h"


//SX1276_LoRaRadio radio(); // this doesn't work!  compiler thinks it's not a SX1276
SX1276_LoRaRadio radio(MBED_CONF_SX1276_LORA_DRIVER_SPI_MOSI,
                       MBED_CONF_SX1276_LORA_DRIVER_SPI_MISO,
                       MBED_CONF_SX1276_LORA_DRIVER_SPI_SCLK,
                       MBED_CONF_SX1276_LORA_DRIVER_SPI_CS,
                       MBED_CONF_SX1276_LORA_DRIVER_RESET,
                       MBED_CONF_SX1276_LORA_DRIVER_DIO0,
                       MBED_CONF_SX1276_LORA_DRIVER_DIO1,
                       MBED_CONF_SX1276_LORA_DRIVER_DIO2,
                       MBED_CONF_SX1276_LORA_DRIVER_DIO3,
                       MBED_CONF_SX1276_LORA_DRIVER_DIO4,
                       MBED_CONF_SX1276_LORA_DRIVER_DIO5,
                       MBED_CONF_SX1276_LORA_DRIVER_RF_SWITCH_CTL1,
                       MBED_CONF_SX1276_LORA_DRIVER_RF_SWITCH_CTL2,
                       MBED_CONF_SX1276_LORA_DRIVER_TXCTL,
                       MBED_CONF_SX1276_LORA_DRIVER_RXCTL,
                       MBED_CONF_SX1276_LORA_DRIVER_ANT_SWITCH,
                       MBED_CONF_SX1276_LORA_DRIVER_PWR_AMP_CTL,
                       MBED_CONF_SX1276_LORA_DRIVER_TCXO);

LoRaWANInterface lorawan(radio);


#define STARTUP_PRINT 1

int indx=0;

void report()
{
    printf("Still running: %d.\r\n",indx++);
}




void lw_event_handler(lorawan_event_t event)
{
    switch (event) {
        case CONNECTED:
            printf("Connected to LW!\n");
            break;
        case DISCONNECTED:
            break;
    }
}


void tx()
{
    uint8_t tx_buffer[30] = "hello world!";
    const int packet_len = 12;
    lorawan.send(MBED_CONF_LORA_APP_PORT, tx_buffer, packet_len, MSG_UNCONFIRMED_FLAG);

}

#define APP_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define DEV_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define APP_KEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

int main(int argc,char *argv[])
{
#if STARTUP_PRINT
    printf("Started the target app.\r\n");
#endif

    EventQueue eq(30 * EVENTS_EVENT_SIZE);
    lorawan.initialize(&eq);

    //set up callbacks: (sufficient?)
    {
        lorawan_app_callbacks_t callbacks ;
        callbacks.events = mbed::callback(lw_event_handler);
        lorawan.add_app_callbacks(&callbacks);
    }


    //start a connection 
    {
        uint8_t
            app_eui[] = APP_EUI,
            dev_eui[] = DEV_EUI,
            app_key[] = APP_KEY;

        lorawan_connect_t connection;
        connection.connect_type = LORAWAN_CONNECTION_OTAA;
        connection.connection_u.otaa.app_eui = app_eui;
        connection.connection_u.otaa.dev_eui = dev_eui;
        connection.connection_u.otaa.app_key = app_key;
        connection.connection_u.otaa.nb_trials = MBED_CONF_LORA_NB_TRIALS;

        lorawan.connect(connection);
    }

    eq.call_every(1s,report);
    eq.call_every(600s,tx);

    eq.dispatch_forever();

    return 0;
}
