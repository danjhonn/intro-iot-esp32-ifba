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

void configuraLED();  //Protótipo da função configuraLED
void controlaLED();   //Protótipo da função controlaLED

//Variáveis globais
String  mensagem; //Armazena a mensagem recebida via comunicação serial
int valorPWM = 0; //Armazena o valor a ser escrito no canal PWM

void setup() {          //O código aqui é executado apenas uma vez, ao iniciar a execução
  configuraLED();       //Realiza a configuração dos pinos e dos canais PWM de cada LED

  Serial.begin(115200); //Inicia a comunicação serial a 115200bps

  //Exibe uma mensagem no monitor serial
  Serial.println("Aguardando seu comando!");
}

void loop() {                                //O código aqui é executado repetidamente
  if(Serial.available()){                    //Se existir algum dado disponível para ser lido
    mensagem = Serial.readStringUntil('\n'); //lê o dado, excetuando a quebra de linha ('\n') e escreve na variável mensagem
    Serial.print("Recebido: ");
    Serial.println(mensagem);       //Exibe o dado recebido no monitor serial
    controlaLED();                  //Interpreta a mensagem recebida via serial e atualiza o LED
  }

  delay(10);                        //Aguarda 10 milissegundos
}

void configuraLED(){       //Realiza a configuração dos pinos e dos canais PWM de cada LED
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

void controlaLED(){                               //Interpreta a mensagem recebida via serial e atualiza o LED
  if(mensagem[0] == 'r' || mensagem[0] == 'R') {  //Se o primeiro caractere da mensagem for r,
    valorPWM = mensagem.substring(1).toInt();     //converte o final da mensagem em um número inteiro
    ledcWrite(R_CH, valorPWM);                    //e escreve no led vermelho
  }
  if(mensagem[0] == 'g' || mensagem[0] == 'G') {  //Se o primeiro caractere da mensagem for g,
    valorPWM = mensagem.substring(1).toInt();     //converte o final da mensagem em um número inteiro
    ledcWrite(G_CH, valorPWM);                    //e escreve no led verde
  }
  if(mensagem[0] == 'b' || mensagem[0] == 'B') {  //Se o primeiro caractere da mensagem for b,
    valorPWM = mensagem.substring(1).toInt();     //converte o final da mensagem em um número inteiro
    ledcWrite(B_CH, valorPWM);                    //e escreve no led azul
  }
  if(mensagem == "alloff"){ //Se a mensagem for "alloff", desliga todos os LEDs
    ledcWrite(R_CH, 0);        
    ledcWrite(G_CH, 0);
    ledcWrite(B_CH, 0);
  }
}
