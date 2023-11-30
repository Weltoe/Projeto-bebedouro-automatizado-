// C++ code
//
#define trig 6
#define echo 7
#define led 13
#define sinal 5
#define botao 4

////////////////////////////MEXAM AQUI/////////////////////////////////////


short pwm=100;  //Digite o valor do pwm (0 à 255) OBS: NAO COLOCAR ABAIXO DE 80 NEM MAIOR QUE 200
short distan_min= 10;  //Define a distância mínima de acionamento do sistema
short distan_max=30;  //Define a distância máxima de acionamento do sistema
long int tempo_inativo=5000;
long int temp_max=10000;  //Define o tempo máximo que pode permanecer ligado.



////////////////////////////////////////////////////////
float duracao, distancia;
long int cont_delay=0;

void setup()
{
  Serial.begin(9600);
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(botao, INPUT);
  pinMode(sinal, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3,LOW);
  digitalWrite(2,LOW);

}

void loop()
{
  dist();
  Serial.println(digitalRead(botao));

  if(digitalRead(botao)==1){
    delay(500);
    cont_delay=0;
    if(digitalRead(botao)==1){
      digitalWrite(2,HIGH);
      digitalWrite(led,HIGH);
      analogWrite(sinal,pwm);
      while(digitalRead(botao)==1 and cont_delay<temp_max){
        Serial.println(digitalRead(botao));
        delay(200);
        cont_delay+=200;
        Serial.println(cont_delay);
      }

    }
    if(cont_delay>=temp_max){
      digitalWrite(2,LOW);
      digitalWrite(led,LOW);
      analogWrite(sinal,0);
      pare();
    }
    else{
      delay(500);
      digitalWrite(2,LOW);
      digitalWrite(led,LOW);
      analogWrite(sinal,0);
    }
  }

  if(distancia>=distan_min && distancia<=distan_max){
    delay(500);
    cont_delay=0;
    if(distancia>=distan_min && distancia<=distan_max){
      digitalWrite(led,HIGH);
      digitalWrite(2,HIGH);
      analogWrite(sinal,pwm);
      while(distancia>=distan_min && distancia<=distan_max and cont_delay<temp_max){
        dist();
        Serial.println(digitalRead(botao));
        cont_delay+=200;
        Serial.println(cont_delay);
      }
    }
    if(cont_delay>=temp_max){
      digitalWrite(2,LOW);
      digitalWrite(led,LOW);
      analogWrite(sinal,0);
      pare();
    }
    else{
      delay(500);
      digitalWrite(2,LOW);
      digitalWrite(led,LOW);
      analogWrite(sinal,0);
    }
  }
  
}

void dist(){
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duracao = pulseIn(echo, HIGH);
  distancia = (duracao*.0343)/2;
  delay(200);
  Serial.println(distancia);
}

void pare(){
  while(digitalRead(botao)==1 || (distancia>=distan_min && distancia<=distan_max)){
    Serial.print("Erro!");
    Serial.println();
    dist();
    Serial.println(digitalRead(botao));
  }
  delay(tempo_inativo);
}
