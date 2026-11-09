// #include <Arduino.h>
// #include <ArduinoJson.h>
// #include <WiFi.h>
// #include <ezTime.h>
// #include <HTTPClient.h>
// #include <LiquidCrystal_I2C.h>

// void conexaoWifi();
// void reConexaoWifi();
// void atualizarJson();

// const String API1 = "https://wttr.in/Sao+Caetano+do+Sul?lang=pt&format=";
// String condicao, nascerSol;
// String infoAPI(String codigo);
// String diaSemanaBR();
// String mesBR();
// int diasRestantesAno();

// JsonDocument dados;

// float temp, sTermica;
// int umi, pressao;

// const char *SSID = "SENAI EDUCACIONAL";
// const char *SENHA = "info@134";

// const unsigned long tempoConexao = 10000;
// const unsigned long tempoReConexao = 5000;
// unsigned long tempoezTime;
// unsigned long auxilio;

// Timezone sp;

// void setup() {
//   Serial.begin(115200);

//   conexaoWifi();

//   if (WiFi.status() == WL_CONNECTED) {
//     sp.setLocation("America/Sao_Paulo");

//     waitForSync();

//     // Serial.printf(
//     //   "Hoje e %s de %s de %s, %s e faltam %i dias para o ano novo.\n",
//     //   String(sp.day()).c_str(),
//     //   mesBR().c_str(),
//     //   sp.dateTime("Y").c_str(),
//     //   diaSemanaBR().c_str(),
//     //   diasRestantesAno()
//     // );
//   }
// }

// void loop() {
//   reConexaoWifi();
//   tempoezTime = millis();

//   if(WiFi.status() == WL_CONNECTED){
//     if (millis() - auxilio >= 6000)
//     {
//       atualizarJson();

//       Serial.println("\n--------- JSON ---------");

//       serializeJsonPretty(dados, Serial);

//       Serial.println("\n------------------------");

//       auxilio=tempoezTime;
//     }

//   }

// }

// void atualizarJson() {

//     dados.clear();

//     dados["data"]["dia"] = diaSemanaBR();
//     dados["data"]["diaMes"] = sp.day();
//     dados["data"]["mes"] = mesBR();
//     dados["data"]["ano"] = sp.year();

//     dados["data"]["diasRestantesAno"] = diasRestantesAno();

//     dados["horario"]["hora"] = sp.dateTime("H:i:s");

//     temp = infoAPI("%t").toFloat();
//     sTermica = infoAPI("%f").toFloat();

//     umi = infoAPI("%h").toInt();
//     pressao = infoAPI("%P").toInt();

//     condicao = infoAPI("%C");
//     nascerSol = infoAPI("%S");

//     dados["clima"]["temperatura"] = temp;
//     dados["clima"]["sensacaoTermica"] = sTermica;

//     dados["clima"]["umidade"] = umi;
//     dados["clima"]["pressao"] = pressao;

//     dados["clima"]["condicao"] = condicao;
//     dados["clima"]["nascerSol"] = nascerSol;
// }

// String diaSemanaBR() {
//   const char* dias[] = {
//     "Domingo", "Segunda-feira", "Terca-feira",
//     "Quarta-feira", "Quinta-feira", "Sexta-feira",
//     "Sabado"
//   };

//   return dias[sp.weekday() - 1];
// }

// String mesBR() {
//   const char* meses[] = {
//     "Janeiro", "Fevereiro", "Marco", "Abril",
//     "Maio", "Junho", "Julho", "Agosto",
//     "Setembro", "Outubro", "Novembro", "Dezembro"
//   };

//   return meses[sp.month() - 1];
// }

// int diasRestantesAno() {
//   int ano = sp.year();

//   int totalDias = 365;

//   if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
//     totalDias = 366;
//   }

//   return totalDias - sp.dayOfYear();
// }

// String infoAPI(String codigo) {
//   HTTPClient http;

//   http.begin(API1 + codigo);

//   int codeHTTP = http.GET();
//   String resposta = "";

//   if (codeHTTP == 200)
//   {
//     resposta= http.getString();
//   }

//   else
//   {
//     Serial.println("Deu erro. Código: " + String(codeHTTP));
//   }

//   http.end();

//   return resposta;
// }

// void conexaoWifi() {
//   Serial.printf("Conectando ao Wi-Fi: %s\n", SSID);

//   WiFi.begin(SSID, SENHA);

//   unsigned long tempoWifi = millis();

//   while (WiFi.status() != WL_CONNECTED &&
//          millis() - tempoWifi < tempoConexao) {
//     Serial.print(".");
//     delay(500);
//   }

//   if (WiFi.status() == WL_CONNECTED) {
//     Serial.println("\nWi-Fi conectado com sucesso!");
//     Serial.print("Endereco IP: ");
//     Serial.println(WiFi.localIP());
//   } else {
//     Serial.println("\nFalha ao se conectar.");
//   }
// }

// void reConexaoWifi() {
//   static unsigned long tempoUltimaConexao = 0;

//   unsigned long tempoAtual = millis();

//   if (tempoAtual - tempoUltimaConexao >= tempoReConexao) {

//     if (WiFi.status() != WL_CONNECTED) {
//       Serial.println("Conexao Wi-Fi perdida!");
//       conexaoWifi();
//     }

//     tempoUltimaConexao = millis();
//   }
// }



#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ezTime.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

void conexaoWifi();
void reConexaoWifi();
void atualizarJson();
void atualizarLCD();
void atualizarRelogio();
void escreverLinha(int linha, String texto);

String infoAPI(String codigo);
String diaSemanaBR();
String mesBR();
int diasRestantesAno();

const String API1 = "https://wttr.in/Sao+Caetano+do+Sul?lang=pt&format=";

const char *SSID = "SENAI EDUCACIONAL";
const char *SENHA = "info@134";

const unsigned long tempoConexao = 10000;
const unsigned long tempoReConexao = 5000;
const unsigned long intervaloClima = 6000;

LiquidCrystal_I2C lcd(0x27, 20, 4);

JsonDocument dados;

String condicao, nascerSol;

float temp, sTermica;
int umi, pressao;

Timezone sp;

String horarioAnterior = "";
String dataAnterior = "";

unsigned long auxilioClima = 0;

bool horarioSincronizado = false;

void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  escreverLinha(0, "Iniciando ESP32...");
  escreverLinha(1, "Conectando Wi-Fi");

  conexaoWifi();

  if (WiFi.status() == WL_CONNECTED)
  {
    escreverLinha(0, "Sincronizando");
    escreverLinha(1, "Horario...");

    sp.setLocation("America/Sao_Paulo");

    waitForSync();

    horarioSincronizado = true;

    escreverLinha(0, "ESP32 conectado!");
    escreverLinha(1, "Carregando dados...");
  }
  else
  {
    escreverLinha(0, "Falha no Wi-Fi");
    escreverLinha(1, "Verifique a rede");
  }
}

void loop()
{
  events();

  reConexaoWifi();

  if (horarioSincronizado)
  {
    atualizarRelogio();
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    if (millis() - auxilioClima >= intervaloClima)
    {
      atualizarJson();

      atualizarLCD();

      Serial.println("\n--------- JSON ---------");
      serializeJsonPretty(dados, Serial);
      Serial.println("\n------------------------");

      auxilioClima = millis();
    }
  }
}

void atualizarRelogio()
{
  String dataAtual = "";

  if (sp.day() < 10)
  {
    dataAtual += "0";
  }

  dataAtual += String(sp.day()) + "/";

  if (sp.month() < 10)
  {
    dataAtual += "0";
  }

  dataAtual += String(sp.month()) + "/";
  dataAtual += String(sp.year());

  String horarioAtual = sp.dateTime("H:i:s");

  if (dataAtual != dataAnterior ||
      horarioAtual != horarioAnterior)
  {
    escreverLinha(0, dataAtual + " " + horarioAtual);

    dataAnterior = dataAtual;
    horarioAnterior = horarioAtual;
  }
}

void atualizarJson()
{
  dados.clear();

  dados["data"]["dia"] = diaSemanaBR();
  dados["data"]["diaMes"] = sp.day();
  dados["data"]["mes"] = mesBR();
  dados["data"]["ano"] = sp.year();

  dados["data"]["diasRestantesAno"] = diasRestantesAno();

  dados["horario"]["hora"] = sp.dateTime("H:i:s");

  temp = infoAPI("%t").toFloat();
  sTermica = infoAPI("%f").toFloat();

  umi = infoAPI("%h").toInt();
  pressao = infoAPI("%P").toInt();

  condicao = infoAPI("%C");
  nascerSol = infoAPI("%S");

  dados["clima"]["temperatura"] = temp;
  dados["clima"]["sensacaoTermica"] = sTermica;

  dados["clima"]["umidade"] = umi;
  dados["clima"]["pressao"] = pressao;

  dados["clima"]["condicao"] = condicao;
  dados["clima"]["nascerSol"] = nascerSol;
}

void atualizarLCD()
{
  String linha2 = "T:" + String(dados["clima"]["temperatura"].as<float>(), 0) + "C ST:" + String(dados["clima"]["sensacaoTermica"].as<float>(), 0) + "C";

  escreverLinha(1, linha2);

  String linha3 = "U:" + String(dados["clima"]["umidade"].as<int>()) + "% P:" + String(dados["clima"]["pressao"].as<int>()) + "hPa";

  escreverLinha(2, linha3);

  String clima = dados["clima"]["condicao"].as<String>();
  String nascer = dados["clima"]["nascerSol"].as<String>();

  clima = clima.substring(0, 11);

  while (clima.length() < 11)
  {
    clima += " ";
  }

  nascer = nascer.substring(0, 8);

  escreverLinha(3, clima + " " + nascer);
}

void escreverLinha(int linha, String texto)
{
  lcd.setCursor(0, linha);

  texto = texto.substring(0, 20);

  lcd.print(texto);

  for (int i = texto.length(); i < 20; i++)
  {
    lcd.print(" ");
  }
}

String diaSemanaBR()
{
  const char *dias[] = {
      "Domingo", "Segunda-feira", "Terca-feira",
      "Quarta-feira", "Quinta-feira", "Sexta-feira",
      "Sabado"};

  return dias[sp.weekday() - 1];
}

String mesBR()
{
  const char *meses[] = {
      "Janeiro", "Fevereiro", "Marco", "Abril",
      "Maio", "Junho", "Julho", "Agosto",
      "Setembro", "Outubro", "Novembro", "Dezembro"};

  return meses[sp.month() - 1];
}

int diasRestantesAno()
{
  int ano = sp.year();

  int totalDias = 365;

  if ((ano % 4 == 0 && ano % 100 != 0) ||
      (ano % 400 == 0))
  {
    totalDias = 366;
  }

  return totalDias - (sp.dayOfYear() + 1);
}

String infoAPI(String codigo)
{
  HTTPClient http;

  http.setConnectTimeout(3000);
  http.setTimeout(3000);

  http.begin(API1 + codigo);

  int codeHTTP = http.GET();

  String resposta = "";

  if (codeHTTP == 200)
  {
    resposta = http.getString();
    resposta.trim();
  }
  else
  {
    Serial.println("Deu erro. Codigo: " + String(codeHTTP));
  }

  http.end();

  atualizarRelogio();

  return resposta;
}

void conexaoWifi()
{
  Serial.printf("Conectando ao Wi-Fi: %s\n", SSID);

  WiFi.begin(SSID, SENHA);

  unsigned long tempoWifi = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - tempoWifi < tempoConexao)
  {
    events();

    Serial.print(".");

    delay(500);

    if (horarioSincronizado)
    {
      atualizarRelogio();
    }
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWi-Fi conectado com sucesso!");

    Serial.print("Endereco IP: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFalha ao se conectar.");
  }
}

void reConexaoWifi()
{
  static unsigned long tempoUltimaConexao = 0;

  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoUltimaConexao >= tempoReConexao)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Conexao Wi-Fi perdida!");

      conexaoWifi();

      if (WiFi.status() == WL_CONNECTED && !horarioSincronizado)
      {
        sp.setLocation("America/Sao_Paulo");
        waitForSync();
        horarioSincronizado = true;
      }
    }

    tempoUltimaConexao = millis();
  }
}