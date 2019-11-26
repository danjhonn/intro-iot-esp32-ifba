//LED RGB - ESP32
//Por: Daniel Jonathas da Costa Pereira

//Definindo os pinos ligados ao LED
#define R_PIN 18      //Pino que controla o LED vermelho
#define G_PIN 19      //Pino que controla o LED verde
#define B_PIN 21      //Pino que controla o LED azul

void setup() {             //O código aqui é executado apenas uma vez, ao iniciar a execução
  pinMode(R_PIN, OUTPUT);  //Define o pino do LED vermelho como saída
  pinMode(G_PIN, OUTPUT);  //Define o pino do LED verde como saída
  pinMode(B_PIN, OUTPUT);  //Define o pino do LED azul como saída

  //Inicia o programa com os LEDs desligados
  digitalWrite(R_PIN, LOW);
  digitalWrite(G_PIN, LOW);
  digitalWrite(B_PIN, LOW);
}

void loop() {                //O código aqui é executado repetidamente
  digitalWrite(R_PIN, HIGH); //Liga o LED vermelho
  delay(1000);               //Espera 1 segundo (1000 milissegundos)
  digitalWrite(R_PIN, LOW);  //Desliga o LED vermelho

  digitalWrite(G_PIN, HIGH); //Liga o LED verde
  delay(1000);               //Espera 1 segundo (1000 milissegundos)
  digitalWrite(G_PIN, LOW);  //Desliga o LED verde

  digitalWrite(B_PIN, HIGH); //Liga o LED azul
  delay(1000);               //Espera 1 segundo (1000 milissegundos)
  digitalWrite(B_PIN, LOW);  //Desliga o LED azul
}
