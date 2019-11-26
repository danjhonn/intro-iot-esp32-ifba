//ESP32 - Bluetooth Low Energy (BLE) + LED RGB
//Por: Daniel Jonathas da Costa Pereira
//
//Código ESP32 baseado nos exemplos de Neil Kolban para o ESP-IDF: 
//      https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
//      https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
//
//Aplicativo Android baseado no exemplo de Timothy Woo (www.botletics.com)

//Importação das bibliotecas necessárias para o BLE (Bluetooth Low Energy)
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <string> //Biblioteca necessária para trabalhar com strings

//Configurações do Bluetooth
BLEServer* pServer = NULL;                   //Ponteiro do servidor
BLECharacteristic* pRXCharacteristic = NULL; //Ponteiro da característica RX
bool deviceConnected = false;                //Variável que indica se há um dispositivo conectado

//Definição dos UUID (Identificador Único Universal)
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define RX_CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define TX_CHARACTERISTIC_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

//Definição do nome do Bluetooth
#define BLE_NAME "MeuESP32"

//Configurações do LED
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
String mensagem;       //Variável que armazena a mensagem recebida via Bluetooth
String mensagemAntiga; //Variável que armazena a mensagem anterior recebida via Bluetooth
int valorPWM = 0;      //Armazena o valor a ser escrito no canal PWM
bool ligaLED = false;  //Define se o LED pode ser ligado ou não

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer){
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer){
    deviceConnected = false;
  }
};

void setup() { //O código aqui é executado apenas uma vez, ao iniciar a execução
  //Inicializando o BLE
  BLEDevice::init(BLE_NAME);

  //Criando o servidor BLE
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  //Criando o serviço BLE
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //Criando a característica BLE RX
  pRXCharacteristic = pService->createCharacteristic(
                        RX_CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_WRITE);

  //Iniciando o serviço
  pService->start();

  //Torna o ESP32 visível
  pServer->getAdvertising()->start();
  
  //Realiza a configuração dos pinos e dos canais PWM de cada LED
  configuraLED();

  //Inicia a comunição serial a 115200bps
  Serial.begin(115200);
  
  Serial.println("Aguardando conexão com um cliente.");
}

void loop() {                 //O código aqui é executado repetidamente
  if(deviceConnected){        //Executa se há um cliente conectado
    //Lendo o valor da característica
    std::string value = pRXCharacteristic->getValue();
    if(value.length() > 0){
      mensagem = "";          //Limpando mensagem
      for(int i = 0; i < (value.length() - 1); i++)
        mensagem += value[i]; //Salvando value em mensagem
    }
    delay(3);
  }

  if(mensagemAntiga != mensagem){ //Se a mensagem estiver atualizada (for diferente da antiga)
    Serial.print("Novo dado recebido: ");
    Serial.println(mensagem);     //Exibe a mensagem no monitor serial

    controlaLED();                //Interpreta a mensagem recebida via Bluetooth e atualiza o LED
  }

  if(!deviceConnected){       //Executa se não há um cliente conectado
    ligaLED = false;          //Atribui o valor "false" à variável ligaLED
    controlaLED();            //Chama a função controlaLED para desligar o LED
    delay(1000);              //Aguarda 1 segundo
  }

  delay(10);
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

  //Chama controlaLED para iniciar o programa com os LEDs desligados
  controlaLED();
}

void controlaLED(){ //Interpreta a mensagem recebida via Bluetooth e atualiza o LED
  if(mensagem[0] == 'R' && ligaLED == true) {      //Se o primeiro caractere da mensagem for R,
    valorPWM = mensagem.substring(1).toInt();      //converte o final da mensagem em um número inteiro
    ledcWrite(R_CH, valorPWM);                     //e escreve no led vermelho
  }
  else if(mensagem[0] == 'G' && ligaLED == true) { //Se o primeiro caractere da mensagem for G,
    valorPWM = mensagem.substring(1).toInt();      //converte o final da mensagem em um número inteiro
    ledcWrite(G_CH, valorPWM);                     //e escreve no led verde
  }
  else if(mensagem[0] == 'B' && ligaLED == true) { //Se o primeiro caractere da mensagem for B,
    valorPWM = mensagem.substring(1).toInt();      //converte o final da mensagem em um número inteiro
    ledcWrite(B_CH, valorPWM);                     //e escreve no led azul
  }
  else if(mensagem == "on"){  //Se a mensagem for "on", permite que o LED seja ligado
    ligaLED = true;
  }
  else if(mensagem == "off"){ //Se a mensagem for "off", não permite que o LED seja ligado
    ligaLED = false;
  }

  if(!ligaLED){ //Se ligaLED for "false", desliga o LED
    ledcWrite(R_CH, 0);
    ledcWrite(G_CH, 0);
    ledcWrite(B_CH, 0);
  }

  mensagemAntiga = mensagem;
}
