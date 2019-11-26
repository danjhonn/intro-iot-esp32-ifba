//ESP32 - WiFi + LED RGB
//Por: Daniel Jonathas da Costa Pereira
//
//Código ESP32 baseado nos exemplos disponíveis em Random Nerd Tutorials:
//    https://randomnerdtutorials.com/esp32-access-point-ap-web-server/
//    https://randomnerdtutorials.com/esp32-web-server-arduino-ide/

#include <WiFi.h> //Biblioteca responsável pelo suporte ao WiFi

//Configurações da rede
const char* ssid     = "ESP32-Access-Point";
const char* password = "12345678";

WiFiServer server(80); //Define o servidor WiFi na porta 80

String header; //Variável que armazena o cabeçalho da solicitação HTTP

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

void configuraLED();                     //Protótipo da função configuraLED
void controlaLED();                      //Protótipo da função controlaLED
void exibePaginaHTML(WiFiClient client); //Protótipo da função exibePaginaHTML

//Variáveis globais
String mensagem;       //Variável que armazena a mensagem recebida via Bluetooth
String mensagemAntiga; //Variável que armazena a mensagem anterior recebida via Bluetooth
int    valorPWM = 0;   //Armazena o valor a ser escrito no canal PWM

void setup() {                    //O código aqui é executado apenas uma vez, ao iniciar a execução
  Serial.begin(115200);           //Inicia comunicação serial a 115200bps

  Serial.println("Configurando ponto de acesso WiFi...");
  WiFi.softAP(ssid,password);     //Inicia o WiFi e configura o ponto de acesso com os dados fornecidos

  IPAddress IP = WiFi.softAPIP(); //Obtendo o endereço IP do ponto de acesso
  Serial.print("Endereço IP do ponto de acesso: ");
  Serial.println(IP);

  server.begin();                 //Inicia o servidor

  configuraLED();                 //Realiza a configuração dos pinos e dos canais PWM de cada LED
}

void loop() {                                 //O código aqui é executado repetidamente
  WiFiClient client = server.available();     //Verificando a conexão de um cliente

  if(client){                                 //Se um novo cliente se conecta,
    Serial.println("Comunicação iniciada");   //exibe uma mensagem no monitor serial
    String dadoCliente = "";                  //String que armazena o dado recebido do cliente

    while(client.connected()){                //Loop enquanto o cliente estiver conectado
      if(client.available()){                 //Se há algum dado para ler do cliente,
        char c = client.read();               //lê o byte,
        Serial.print(c);                      //e então exibe no monitor serial
        header += c;
        if(c == '\n'){                        //Identifica o fim da solicitação HTTP do cliente
          if(dadoCliente.length() == 0){
            //Os cabeçalhos HTTP sempre começam com um código de resposta
            //(por exemplo, HTTP / 1.1 200 OK) e um tipo de conteúdo para
            //que o cliente saiba o que está por vir e, em seguida,
            //uma linha em branco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //Interpreta a solicitação HTTP
            if(header.indexOf("GET") >= 0){ //Identifica uma requisição de controle do LED
              //Salva em mensagem uma substring com o dado recebido
              mensagem = header.substring(header.indexOf("GET /") + 5, header.indexOf(" HTTP"));
              Serial.print("Novo dado recebido: ");
              Serial.println(mensagem);     //Exibe a mensagem no monitor serial
              controlaLED();
            }

            exibePaginaHTML(client); //Exibe a página HTML

            //A resposta da solicitação HTTP termina com uma linha em branco
            client.println();
            //Encerra o loop while
            break;
          } else {
            dadoCliente = "";
          }
        } else if(c != '\r'){ //Se for recebido algum caractere além do caractere
          dadoCliente += c;   //retorno de carro, \r, armazena-o em dadoCliente
        }
      }
    }

    header = "";   //Limpa a variável header (cabeçalho da solicitação HTTP)
    client.stop(); //Encerra a conexão com o cliente
    Serial.println("Comunicação encerrada");
  }
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

void controlaLED(){ //Interpreta a mensagem recebida via WiFi e atualiza o LED
  if(mensagem[0] == 'R') {                    //Se o primeiro caractere da mensagem for R,
    valorPWM = mensagem.substring(1).toInt(); //converte o final da mensagem em um número inteiro
    ledcWrite(R_CH, valorPWM);                //e escreve no led vermelho
  }
  else if(mensagem[0] == 'G') {               //Se o primeiro caractere da mensagem for G,
    valorPWM = mensagem.substring(1).toInt(); //converte o final da mensagem em um número inteiro
    ledcWrite(G_CH, valorPWM);                //e escreve no led verde
  }
  else if(mensagem[0] == 'B') {               //Se o primeiro caractere da mensagem for B,
    valorPWM = mensagem.substring(1).toInt(); //converte o final da mensagem em um número inteiro
    ledcWrite(B_CH, valorPWM);                //e escreve no led azul
  }

  mensagemAntiga = mensagem;
}

void exibePaginaHTML(WiFiClient client){ //Exibe a página HTML
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>ESP32</title>"); //Título da página
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<style>");
  client.println("    html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println("    .slidecontainer {");
  client.println("    margin-top: 50px;");
  client.println("    margin-bottom: 50px;");
  client.println("    width: 100%; /* Width of the outside container */a");
  client.println("    }");
  client.println("    /* The slider itself */");
  client.println("    .slider {");
  client.println("    -webkit-appearance: none;  /* Override default CSS styles */");
  client.println("    appearance: none;");
  client.println("    width: 50%; /* Full-width */");
  client.println("    height: 15px; /* Specified height */");
  client.println("    border-radius: 15px;");
  client.println("    background: #d3d3d3; /* Grey background */");
  client.println("    outline: none; /* Remove outline */");
  client.println("    opacity: 0.7; /* Set transparency (for mouse-over effects on hover) */");
  client.println("    -webkit-transition: .2s; /* 0.2 seconds transition on hover */");
  client.println("    transition: opacity .2s;");
  client.println("    }");
  client.println("    /* Mouse-over effects */");
  client.println("    .slider:hover {");
  client.println("    opacity: 1; /* Fully shown on mouse-over */");
  client.println("    }");
  client.println("    /* The slider handle (use -webkit- (Chrome, Opera, Safari, Edge) and -moz- (Firefox) to override default look) */");
  client.println("    .slider::-webkit-slider-thumb {");
  client.println("    -webkit-appearance: none; /* Override default look */");
  client.println("    appearance: none;");
  client.println("    width: 25px; /* Set a specific slider handle width */");
  client.println("    height: 25px; /* Slider handle height */");
  client.println("    border-radius: 50%;");
  client.println("    background: #4CAF50; /* Green background */");
  client.println("    cursor: pointer; /* Cursor on hover */");
  client.println("    }");
  client.println("    .slider::-moz-range-thumb {");
  client.println("    width: 25px; /* Set a specific slider handle width */");
  client.println("    height: 25px; /* Slider handle height */");
  client.println("    border-radius: 50%;");
  client.println("    background: #4CAF50; /* Green background */");
  client.println("    cursor: pointer; /* Cursor on hover */");
  client.println("    }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1>Introdu&ccedil&atildeo &agrave Internet das Coisas Usando ESP32</h1>");
  client.println("<h5 style=\"margin-top:50px;\">Mova os controles deslizantes para variar a cor do LED</h5>");
  client.println("<div class=\"text\" style=\"margin-top:40px;\">");
  client.println("    <output type=\"text\" id=\"values\">");
  client.println("</div>");
  client.println("<div class=\"slidecontainer\">");
  client.println("    <input oninput=\"requisicaoHTTP('R',this.value); atualizaTexto();\" type=\"range\" min=\"0\" max =\"256\" value=\"0\" class=\"slider\" id=\"RAdjust\">");
  client.println("</div>");
  client.println("<div class=\"slidecontainer\">");
  client.println("    <input oninput=\"requisicaoHTTP('G',this.value); atualizaTexto();\" type=\"range\" min=\"0\" max =\"256\" value=\"0\" class=\"slider\" id=\"GAdjust\">");
  client.println("</div>");
  client.println("<div class=\"slidecontainer\">");
  client.println("    <input oninput=\"requisicaoHTTP('B',this.value); atualizaTexto();\" type=\"range\" min=\"0\" max =\"256\" value=\"0\" class=\"slider\" id=\"BAdjust\">");
  client.println("</div>");
  client.println("<div class=\"text\">");
  client.println("    <output type=\"text\" id=\"textV\">");
  client.println("</div>");
  client.println("<script>");
  client.println("var R = document.getElementById(\"RAdjust\");");
  client.println("var G = document.getElementById(\"GAdjust\");");
  client.println("var B = document.getElementById(\"BAdjust\");");
  client.println("var values = document.getElementById(\"values\");");
  client.println("atualizaTexto();");
  client.println("function atualizaTexto(){");
  client.println("    values.innerHTML = \"R: \" + R.value + \" G: \" + G.value + \" B: \" + B.value;");
  client.println("}");
  client.println("function requisicaoHTTP(color, value){");
  client.println("    var xhttp = new XMLHttpRequest();");
  client.println("    xhttp.open(\"GET\", color + value, true);");
  client.println("    xhttp.send();");
  client.println("}");
  client.println("</script>");
  client.println("</body>");
  client.println("</html>");
}
