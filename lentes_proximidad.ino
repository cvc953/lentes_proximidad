const int trigPin = 9;
const int echoPin = 10;
const int BUZZER  = 11;

unsigned long ultimoBeep = 0;

const int DIST_PELIGRO    = 60;
const int DIST_PRECAUCION = 100;
const int DIST_LIBRE      = 150;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH); // asegurar silencio al arrancar

  Serial.begin(9600);

  // Pitido de confirmación
  digitalWrite(BUZZER, LOW); delay(100);
  digitalWrite(BUZZER, HIGH);  delay(150);
  digitalWrite(BUZZER, LOW); delay(100);
  digitalWrite(BUZZER, HIGH);  delay(150);
  digitalWrite(BUZZER, LOW); delay(200);
  digitalWrite(BUZZER, HIGH);  delay(300);
}

void loop() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);

  long duration  = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  emitirAlerta(distance);
  delay(100);
}

void emitirAlerta(float distance) {
  unsigned long ahora = millis();

  if (distance <= DIST_PELIGRO) {
    // PELIGRO: pitido continuo
    digitalWrite(BUZZER, LOW);
    Serial.print(" Peligro");

  } else if (distance <= DIST_PRECAUCION) {
    // PRECAUCIÓN: beeps rápidos
    digitalWrite(BUZZER, HIGH); // cortar tono continuo
    int intervalo = map((int)distance, DIST_PELIGRO, DIST_PRECAUCION, 150, 500);
    Serial.print(" Precaución");
    if (ahora - ultimoBeep >= (unsigned long)intervalo) {
      ultimoBeep = ahora;
      digitalWrite(BUZZER, LOW); delay(80);
      digitalWrite(BUZZER, HIGH);
    }

  } else if (distance <= DIST_LIBRE) {
    // AVISO: beep lento
    digitalWrite(BUZZER, HIGH);
    if (ahora - ultimoBeep >= 900UL) {
      ultimoBeep = ahora;
      digitalWrite(BUZZER, LOW); delay(60);
      digitalWrite(BUZZER, HIGH);
    }

  } else {
    // LIBRE: silencio
    digitalWrite(BUZZER, HIGH);
  }
}
