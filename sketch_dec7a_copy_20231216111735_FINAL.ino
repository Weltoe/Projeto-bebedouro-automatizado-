// C++ CODE



///////// PROJETO BEBEDOURO AUTOMÁTICO/////////////////////
//INTRODUÇÃO A ENGENHARIA ELÉTRICA E ALGORÍTIMO E LÓGICA DE PROGRAMAÇÃO


// /////////////////////DEFININDOS OS PINOS////////////////////
#define trig 6
#define echo 7
#define led 13
#define sinal 5
#define botao 4

////////////////////////////INICIALIZANDO VARIÁVEIS/////////////////////////////////////

short pwm=100;  //SINAL DO PWM RESPONSÁVEL POR CONTROLAR A POTENCIA DO MOTOR ATRAVÉS DA PONTE H
short distan_min= 10;  //DEFINE A DISTÂNCIA MÍNIMA DE ACIONAMENTO DO SISTEMA
short distan_max=30;  //DEFINE A DISTÂNCIA MÁXIMA DE ACIONAMENTO DO SISTEMA
long int tempo_inativo=5000; //DEFINE O TEMPO QUE VAI FICAR INOPERANTE APÓS ESTADO DE ERRO
long int temp_max=10000;  //DEFINE O TEMPO MÁXIMO QUE PODE PERMANECER LIGADO.

float duracao, distancia; ///VARIAVEIS RESPONSÁVEIS POR CALCULAR A DISTANCIA DO SENSOR 
long int cont_delay=0; //CONTA O TEMPO AO ACIONAR O MOTOR PELO SENSOR OU PELOS BOTÕES

void setup()
{

  ////////////////////DEFININDO ENTRADAS, SAIDAS E ESTADOS LÓGICOS////////////////////////
 
  Serial.begin(9600);   //O MONITOR SERIAL É ACIONADO
  pinMode(echo, INPUT); //PINO QUE DETECTA O PULSO RETONADO PELO SENSOR É DEFINIDO COMO ENTRADA
  pinMode(trig, OUTPUT); //PINO QUE ENVIA UM PULSO PARA O SENSOR É DEFINIDO COMO SAÍDA
  pinMode(botao, INPUT); //PINO DO BOTÃO É CONFIGURADO COMO INPUT
  pinMode(sinal, OUTPUT);  //PINO ONDE É ENVIADO O SINAL PWM PARA PONTE H
  pinMode(led, OUTPUT);  ////SAIDA QUE INDICA O BEBEDEOURO EM ESTADO DE ERRO

  /////PINOS 2 E 3 SÃO RESPONSÁVEIS POR COMANDAR O SENTIDO E ATIVAÇÃO DO MOTOR, PORTANTO DEVEM ESTAR EM ESTADO 0
  pinMode(2, OUTPUT);     //INDICA O SENTIDO DO MOTOR
  pinMode(3, OUTPUT);     //INDICA O SENTIDO DO MOTOR 
  digitalWrite(3,LOW);    //É MANTIDO EM ESTADO BAIXO
  digitalWrite(2,LOW);    //É MANTIDO EM ESTADO BAIXO

}

void loop()
{   //INICIALMENTE O SENSOR PRECISA ESTAR ATIVO USANDO A FUNÇÃO DIST(); PARA CALCULAR A DISTANCIA DE DETECÇÃO
  dist();
  Serial.println(digitalRead(botao));    ///PRINTA O ESTADO DO BOTÃO NO MONITOR

  ///////ACIONAMENTO ATRAVES DO ESTADO DO BOTÃO///////////////
  if(digitalRead(botao)==1){       //SE BOTÃO PRECIONADO, ENTRE E FAÇA OS COMANDOS ABAIXO
    delay(500);                   //  DELAY PARA ACIONAMENTO              
    cont_delay=0;                 // VARIAVEL CONTADORA ZERADA A CADA CICLO
    if(digitalRead(botao)==1){    // SE REALMENTE ESTA PRECIONADO, CONTINUE
      digitalWrite(2,HIGH);       // O MOTOR ESTÁ APTO PARA SER ACIONADO
      analogWrite(sinal,pwm);     // SINAL RECEBE PWM
      while(digitalRead(botao)==1 and cont_delay<temp_max){  // ENQUANTO BOTÃO PRECIONADO E CONTADOR FOR MENOR QUE O TEMPO MAXIMO ESTIPULADO
        Serial.println(digitalRead(botao));                //PRINT O ESTADO DO BOTÃO
        delay(200);    
        cont_delay+=200;                                  // CONTE O TEMPO
        Serial.println(cont_delay);                      //MOSTRE A CONTAGEM NA TELA
      }

    }
    if(cont_delay>=temp_max){   //SE OS SINAIS DE ENTRADA PERMANECERAM ATÉ O TEMPO MÁXIMO SER ATINGIDO
      pare();  //A FUNÇÃO PARE É ACESSADA
    }
    else{        //SE NÃO, OS SINAIS DE ENTRADA FORAM RETIRADOS ANTES DO TEMPO MÁXIMO SER ATINGIDO.
      delay(500); //DEMORA 500 MILISEGUNDOS
      digitalWrite(2,LOW);  //O SINAL ALTO REFERNETE AO SENTIDO É PARADO
      analogWrite(sinal,0);  //O PULSO PWM É PARADO
    }
  }
  
  
  ///////ACIONAMENTO ATRAVES DO ESTADO DO SENSOR ULTRASSÔNICO///////////////
  if(distancia>=distan_min && distancia<=distan_max){  //SE DISTANCIA CALCULADA ESTIVER ENTRE A ESTIPULADA, ENTRE E FAÇA OS COMANDOS ABAIXO
    delay(500);                                             //  DELAY PARA ACIONAMENTO 
    cont_delay=0;                                         // VARIAVEL CONTADORA ZERADA A CADA CICLO
    if(distancia>=distan_min && distancia<=distan_max){    // SE REALMENTE A DISTANCIA ESTA CERTA, CONTINUE
      digitalWrite(2,HIGH);                                // O MOTOR ESTÁ APTO PARA SER ACIONADO
      analogWrite(sinal,pwm);                              // SINAL RECEBE PWM
      while(distancia>=distan_min && distancia<=distan_max and cont_delay<temp_max){ // ENQUANTO DISTANCIA ESTIVER DENTRO DO INTERVALO E CONTADOR FOR MENOR QUE O TEMPO MAXIMO ESTIPULADO
        dist();                                                    //ATUALIZE O VALOR DA VARIAVEL DISTANCIA
        Serial.println(digitalRead(botao));                        //MOSTRE A CONDIÇÃO DO BOTÃO
        cont_delay+=200;                                           // CONTE O TEMPO
        Serial.println(cont_delay);                               //MOSTRE A CONTAGEM NA TELA
      }
    }
    if(cont_delay>=temp_max){    //SE OS SINAIS DE ENTRADA PERMANECERAM ATÉ O TEMPO MÁXIMO SER ATINGIDO
      pare();                     //A FUNÇÃO PARE(); É ACESSADA
    }
    else{                        //SE NÃO, OS SINAIS DE ENTRADA FORAM RETIRADOS ANTES DO TEMPO MÁXIMO SER ATINGIDO.
      delay(500);                //DEMORA 500 MILISEGUNDOS
      digitalWrite(2,LOW);          //O SINAL ALTO REFERNETE AO SENTIDO É PARADO
      analogWrite(sinal,0);           //O PULSO PWM É PARADO
    }
  }
  
}

void dist(){
  digitalWrite(trig, HIGH);   //É ENVIADO UM SINAL ALTO PARA O TRIG
  delayMicroseconds(10);      //O ESTADO ALTO É MANTIDO DURANTE 10 MICROSEGUNDOS
  digitalWrite(trig, LOW);    //O SINAL ALTO É INTERROMPIDO

  duracao = pulseIn(echo, HIGH);  //DURACAO RECEBE O TEMPO DE RETORNO DO PULSO ENVIADO PELO ECHO
  distancia = (duracao*.0343)/2;     //DISTANCIA CALCULADA CONFORME A VELOCIDADE DO SOM
  delay(200);           //DELAY DE 200 MILISEGUNDOS
  Serial.print("Distancia: ");  
  Serial.println(distancia);    //A DISTÂNCIA É MOSTRADA NO MONITOR SERIAL
}

void pare(){
  ////////////////A BOMBA É DESLIGADA////////////////////////////
  digitalWrite(2,LOW);    //O SINAL ALTO REFERNETE AO SENTIDO É PARADO
  analogWrite(sinal,0);   //O PULSO PWM É PARADO
  digitalWrite(led,1); //ACIONA LED DO PINO 13 INDICANDO ERRO


  while(digitalRead(botao)==1 || (distancia>=distan_min && distancia<=distan_max)){ // ENQUANTO BOTÃO CONTINUA PRECIONADO OU SENSOR AINDA DETECTA O OBJETO
    Serial.print("Erro!");    //"ERRO" É PRINTADO NO MONITOR SERIAL, INDICANDO O ESTADO
    Serial.println();
    dist();  //A FUNÇÃO dist(); É ACESSADA PARA ATUALIZAR O VALOR DA VARIÁVEL distancia
    Serial.print("Estado do botao: ");
    Serial.println(digitalRead(botao));   //O ESTADO DO BOTÃO É MOSTRADO NO MONITOR SERIAL
  }
  delay(tempo_inativo);     
  digitalWrite(led,0);    //SE SAIR DO ERRO CONTE 5 SEGUNDOS DESLIGUE O LED E A FUNÇÃO loop() É RETORNADA;
}
