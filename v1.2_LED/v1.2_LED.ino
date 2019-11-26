//LED RGB - ESP32
//Por: Daniel Jonathas da Costa Pereira

//Definindo os pinos ligados ao LED
#define R_PIN 18      //Pino que controla o LED vermelho
#define G_PIN 19      //Pino que controla o LED verde
#define B_PIN 21      //Pino que controla o LED azul

//Definindo os canais PWM de cada LED (0-15)
#define R_CH 0        //Canal PWM do LED vermelho
#define G_CH 1        //Canal PWM do LED verde
#define B_CH 2        //Canal PWM do LED azul

//Configurando o PWM
#define PWM_FREQ 1000 //Frequência do PWM - 1000Hz
#define PWM_RES  8    //Resolução  do PWM - 8 bits

void setup() {             //O código aqui é executado apenas uma vez, ao iniciar a execução
  pinMode(R_PIN, OUTPUT);  //Define o pino do LED vermelho como saída
  pinMode(G_PIN, OUTPUT);  //Define o pino do LED verde como saída
  pinMode(B_PIN, OUTPUT);  //Define o pino do LED azul como saída

  //Associa cada pino do LED a seu respectivo canal de PWM
  ledcAttachPin(R_PIN, R_CH);
  ledcAttachPin(G_PIN, G_CH);
  ledcAttachPin(B_PIN, B_CH);

  //Configura o canal de PWM de cada LED na frequência e resolução desejadas
  ledcSetup(R_CH, PWM_FREQ, PWM_RES);
  ledcSetup(G_CH, PWM_FREQ, PWM_RES);
  ledcSetup(B_CH, PWM_FREQ, PWM_RES);

  //Inicia o programa com os LEDs desligados
  ledcWrite(R_CH, 0);
  ledcWrite(G_CH, 0);
  ledcWrite(B_CH, 0);
}

void loop() {                //O código aqui é executado repetidamente
  int r = 0, g = 0, b = 0;

  for(r = 0; r < 256; r++){ //Varia o brilho do LED vermelho, iniciando desligado (0) e terminando ligado (255)
    ledcWrite(R_CH,r);      //Escreve o valor do PWM no canal especificado
    delay(5);               //Espera 5 milissegundos
  }
  for(g = 0; g < 256; g++){ //Varia o brilho do LED verde, iniciando desligado (0) e terminando ligado (255)
    ledcWrite(G_CH,g);
    delay(5);
  }
  for(b = 0; b < 256; b++){ //Varia o brilho do LED azul, iniciando desligado (0) e terminando ligado (255)
    ledcWrite(B_CH,b);
    delay(5);
  }

  for(r = 255; r >= 0; r--){ //Varia o brilho do LED vermelho, iniciando ligado (255) e terminando desligado (0)
    ledcWrite(R_CH,r);
    delay(5);
  }
  for(g = 255; g >= 0; g--){ //Varia o brilho do LED verde, iniciando ligado (255) e terminando desligado (0)
    ledcWrite(G_CH,g);
    delay(5);
  }
  for(b = 255; b >= 0; b--){ //Varia o brilho do LED azul, iniciando ligado (255) e terminando desligado (0)
    ledcWrite(B_CH,b);
    delay(5);
  }
}
