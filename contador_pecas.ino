
#include <LiquidCrystal_I2C.h>

#include <Wire.h>
#include <ESP8266WiFi.h>


// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x3F, 16, 2);
WiFiClientSecure client;//Cria um cliente seguro (para ter acesso ao HTTPS)
//String textFix = "GET /forms/d/e/1FAIpQLSdxa1jTr9OP7qSBZF78s9LiOz1b6FogQIZYSCk4UNAFXi3wUQ/formResponse?ifq&entry.1429916403=";

String textFix = "GET /forms/d/e/1FAIpQLSdxa1jTr9OP7qSBZF78s9LiOz1b6FogQIZYSCk4UNAFXi3wUQ/formResponse?ifq&entry.1429916403=1&entry.519880667=";
//Essa String sera uma auxiliar contendo o link utilizado pelo GET, para nao precisar ficar re-escrevendo toda hora
 
int pino_sensor = D7;
int valor = 1; //Variavel leitura sensor
int contador = 0;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);//Habilita o modo estaçao
  WiFi.begin("PRODUCAO", "70115e5c60");//Conecta na rede
    
  delay(2000);//Espera um tempo para se conectar no WiFi
  //Define o pino do sensor como entrada
  pinMode(pino_sensor, INPUT);
  //Inicializa o display LCD 16x2 I2C
   lcd.init();   // INICIALIZA O DISPLAY LCD
  lcd.backlight(); // HABILITA O BACKLIGHT (LUZ DE FUNDO
 
  //informacoes iniciais
  lcd.setCursor(0, 0);
  lcd.print("Entrada de Pecas");
  lcd.setCursor(0, 1);
  lcd.print("Contador: 0");
  Serial.println("Contador com sensor indutivo");

}

void loop()
{
  //Le o pino do sensor
  valor = digitalRead(pino_sensor);
  //Verifica se algum objeto foi detectado (valor = 0)
  if (valor == 0)
  {
    //Incrementa o contador
    contador++;
    //Mostra o valor do contador no display
    lcd.setCursor(0, 1);
   lcd.print("Contador: 0");
    lcd.setCursor(10, 1);
    lcd.print(contador);
    //Mostra o valor do contador no serial monitor
    Serial.print("Contador: ");
    Serial.println(contador);
    //Loop caso o objeto fique parado em frente ao sensor
    while (digitalRead(pino_sensor) == 0)
    {
      if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
    {
        String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
        toSend += contador;//Adiciona valor contador
        toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.
        client.println(toSend);//Enviamos o GET ao servidor-
        client.println("Host: docs.google.com");//-
        client.println();//-
        client.stop();//Encerramos a conexao com o servidor
        Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
                     
    }
    else
    {
        Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
        lcd.setCursor(0, 1);
        lcd.print("Erro Conexao");
    }
      delay(10000);
    }
  }
}
