/*
 * IRremoteESP8266: DumbIRRepeater.ino - Record and playback IR codes.
 * Copyright 2019 David Conran (crankyoldgit)
 *
 * This program will try to capture incoming IR messages and replay them back.
 * It doesn't use any of the advanced detection features, thus it will just
 * replay the messages at fixed modulated frequency (kFrequency) and a 50% duty
 * cycle.
 *
 * Note:
 *   This might NOT be the frequency of the incoming message, so some replayed
 *   messages may not work. The frequency of incoming messages & duty cycle is
 *   lost at the point of the Hardware IR demodulator. The ESP can't see it.
 *
 *                               W A R N I N G
 *   This code is just for educational/example use only. No help will be given
 *   to you to make it do something else, or to make it work with some
 *   weird device or circuit, or to make it more usable or practical.
 *   If it works for you. Great. If not, Congratulations on changing/fixing it.
 *
 * An IR detector/demodulator must be connected to the input, kRecvPin.
 * An IR LED circuit must be connected to the output, kIrLedPin.
 *
 * Example circuit diagrams (both are needed):
 *  https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-receiving
 *  https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-sending
 *
 * Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
 *     have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
 *     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Some digital camera/phones can be used to see if the IR LED is flashed.
 *     Replace the IR LED with a normal LED if you don't have a digital camera
 *     when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP will interfere.
 *     * Pin 16/D0: Has no interrupts on the ESP8266, so can't be used for IR
 *       receiving with this library.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 *
 * Changes:
 *   Version 1.0: June, 2019
 *     - Initial version.
 */

#include <Arduino.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

// ==================== start of TUNEABLE PARAMETERS ====================

// The GPIO an IR detector/demodulator is connected to. Recommended: 14 (D5)
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 14;

// GPIO to use to control the IR LED circuit. Recommended: 4 (D2).
const uint16_t kIrLedPin = 4;

// The Serial connection baud rate.
// NOTE: Make sure you set your Serial Monitor to the same speed.
const uint32_t kBaudRate = 9600;

// As this program is a special purpose capture/resender, let's use a larger
// than expected buffer so we can handle very large IR messages.
// i.e. Up to 512 bits.
const uint16_t kCaptureBufferSize = 1024;

// kTimeout is the Nr. of milli-Seconds of no-more-data before we consider a
// message ended.
const uint8_t kTimeout = 50;  // Milli-Seconds

// kFrequency is the modulation frequency all messages will be replayed at.
const uint16_t kFrequency = 38000;  // in Hz. e.g. 38kHz.

// ==================== end of TUNEABLE PARAMETERS ====================

// The IR transmitter.
IRsend irsend(kIrLedPin);
// The IR receiver.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);
// Somewhere to store the captured message.
decode_results results;
uint16_t rawArray[] = {4464, 4378, 564, 1592, 564, 540, 542, 1588, 570, 1612, 544, 534, 538, 540, 544, 1586, 570, 534,
                       538, 540, 544, 1586, 570, 534, 540, 540, 544, 1586, 570, 1586, 572, 506, 566, 1590, 566, 1590,
                       566,
                       1590, 566, 1592, 566, 1592, 566, 1592, 566, 1592, 566, 1592, 566, 1592, 566, 510, 572, 506, 566,
                       512, 562, 516, 566, 510, 562, 516, 568, 510, 562, 516, 568, 1588, 568, 1588, 568, 510, 564, 1592,
                       564, 512, 570, 508, 564, 512, 570, 508, 566, 512, 570, 508, 566, 1592, 566, 512, 572, 1586, 560,
                       1596, 560, 1596, 560, 1596, 560, 5200, 4432, 4410, 562, 1594, 562, 516, 568, 1588, 570, 1588,
                       568,
                       510, 562, 516, 568, 1588, 568, 508, 564, 514, 570, 1588, 568, 510, 564, 512, 570, 1588, 570,
                       1586,
                       570, 508, 566, 1592, 566, 1592, 566, 1590, 566, 1590, 566, 1590, 568, 1590, 568, 1590, 568, 1588,
                       570, 1588, 568, 508, 564, 514, 568, 508, 564, 514, 570, 508, 566, 512, 570, 506, 566, 538, 544,
                       1586, 572, 1586, 572, 506, 566, 1592, 566, 538, 536, 542, 542, 536, 536, 514, 568, 510, 564, 514,
                       568, 1588, 570, 534, 538, 1594, 564, 1592, 564, 1592, 566, 1592, 566, 5196, 4458, 4384, 568,
                       1590,
                       568, 1590, 568, 510, 564, 1596, 562, 514, 568, 1588, 568, 510, 564, 1594, 562, 540, 544, 508,
                       564,
                       514, 570, 508, 564, 514, 570, 508, 564, 512, 570, 1588, 570, 534, 538, 512, 570, 508, 566, 512,
                       570, 508, 564, 512, 572, 506, 566, 512, 572, 506, 566, 538, 544, 534, 540, 538, 534, 544, 540,
                       538,
                       536, 544, 540, 538, 534, 542, 540, 538, 536, 542, 542, 536, 536, 542, 542, 536, 536, 542, 542,
                       536,
                       536, 1594, 564, 1592, 564, 542, 542, 1588, 570, 534, 538, 1592, 564, 1592, 566, 540, 544
};

// This section of code runs only once at start-up.
void setup() {
    irrecv.enableIRIn();  // Start up the IR receiver.
    irsend.begin();       // Start up the IR sender.

    Serial.begin(kBaudRate, SERIAL_8N1);
    while (!Serial)  // Wait for the serial connection to be establised.
        delay(50);
    Serial.println();

    Serial.print("DumbIRRepeater is now running and waiting for IR input "
                 "on Pin ");
    Serial.println(kRecvPin);
    Serial.print("and will retransmit it on Pin ");
    Serial.println(kIrLedPin);
}

// The repeating section of the code
void loop() {
    irsend.sendRaw(rawArray, 299, kFrequency);
    delay(2000);
}
