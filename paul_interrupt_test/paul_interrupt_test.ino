int pin = 11;
volatile int state = LOW;

void setup() {
    pinMode(pin, OUTPUT);
    attachInterrupt(pin, blink, CHANGE);
    interrupts();
}

void loop() {
    digitalWrite(pin, state);
}

void blink() {
    state = !state;
}
