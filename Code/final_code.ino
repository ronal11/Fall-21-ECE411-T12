const int MAX_LEVEL = 100;      // initialize max number of leds in a sequence
int sequence[MAX_LEVEL];        // led pattern thats generated, contains integer values 1-4 representing leds
int your_sequence[MAX_LEVEL];   // buttons user pushes,contains integer values 1-4 representing leds
int level = 2;                  // initial ammount of leds in a sequence, gets reset back to this value after user loses increases by one after each subsequent win
int gen = 0;                    // indicates whether pattern has been generated
int wrong_pattern = 0;          // indicates the wrong button is pressed in the sequence
int gamemode = 0;               // indicates what game mode user wants to play


//orange
#define OR_LED 22               // led and button pin assignments
#define OR_B 14
//red
#define RED_LED 23
#define RED_B 15
//Blue
#define BLUE_LED 25
#define BLUE_B 17
//Yellow
#define YEL_LED 26
#define YEL_B 18

void show_sequence();     // function displays generated sequence to user
void get_sequence();      // function gets input from user 
void generate_sequence(); // function generates led sequence
void wrong_sequence();    // blinks red led when user sequence is incorrect
void right_sequence();    // flashes all leds when user sequence is correct

int debounce_btn();

int velocity = 1500;            // speed of leds turning on and off when showing sequence,(aka difficulty)

void setup() {

    Serial.begin(9600);       // configuring buttons as input
    pinMode(OR_B, INPUT);
    pinMode(RED_B, INPUT);
    pinMode(BLUE_B, INPUT);
    pinMode(YEL_B, INPUT);

    pinMode(OR_LED, OUTPUT);  // configuring leds as output
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(YEL_LED, OUTPUT);

    digitalWrite(OR_LED, LOW);  // making sure output to leds is low
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(YEL_LED, LOW);
    gen = 0;
    level = 2;
}

void loop()
{
    get_gamemode();
    while(1) {
      if(gen == 0)                                    // if no sequence has been generated...
      {
        generate_sequence();                          // function generates a sequence for user;
        if (level==2) {Serial.print("press any button to start\n");}
        gen = 1;                                      // won't generate another sequence until user enters sequence
      }
  
      if (digitalRead(RED_B) == HIGH || digitalRead(OR_B) == HIGH || digitalRead(BLUE_B) == HIGH || digitalRead(YEL_B) == HIGH || (level > 2))     //If start button is pressed...
      {
        if (level == 2) {
        Serial.print("New game started, showing sequence\n");
        } else { Serial.print("showing next level sequence\n"); }
        show_sequence();                                    //show the sequence
        Serial.print("Enter sequence\n");
        get_sequence();                                    //wait for user to input their sequence
        if(!wrong_pattern) {
          right_sequence();                               // blink all leds
        }
        else {
          wrong_sequence();                               // else, blink red led
        }
        gen = 0;                                          // generate another sequence for user
      }
      if (gamemode == 0) { break; }
    }


}

void show_sequence()                                // function displays generated sequence to user
{
    digitalWrite(OR_LED, LOW);                      // make sure all leds are off
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(YEL_LED, LOW);
    delay(2000);

    for (int i = 0; i < level; i++)                 // for each led in the sequence...
    {
      if(sequence[i] == 1)                         // if value in sequence[i] equals 1
      {
        Serial.print("lighting up red\n");
        digitalWrite(RED_LED, HIGH);               // turn on corresponding led (red)
        delay(velocity);                           // wait "velocity" amount of time until turning off (shorter time = higher difficulty)
        digitalWrite(RED_LED, LOW);                // turn off led
        delay(200);
      }
      if(sequence[i] == 2)                         // if value equals 2
      {
         Serial.print("lighting up orange\n");
        digitalWrite(OR_LED, HIGH);                // turn on orange led
        delay(velocity);                           // wait
        digitalWrite(OR_LED, LOW);                 // turn off led
        delay(200);
      }
      if(sequence[i] == 3)                        // if value equals 3
      {
         Serial.print("lighting up yellow\n");
        digitalWrite(YEL_LED, HIGH);              // turn on yellow led
        delay(velocity);                          // wait
        digitalWrite(YEL_LED, LOW);               // turn off led
        delay(200);
      }
      if(sequence[i] == 4)                        // if value equals 4
      {
        Serial.print("lighting up blue\n");
        digitalWrite(BLUE_LED, HIGH);             // turn on blue led
        delay(velocity);                          // wait
        digitalWrite(BLUE_LED, LOW);              // turn off led
        delay(200);
      }
    }

}

void get_sequence()                             // function receives input from user
{
  int flag = 0;                                 // indicates when input for that led in the sequence has been inputted
  wrong_pattern = 0;                            // reset wrong_pattern flag

  for (int i = 0; i < level; i++)               // for each led in the sequence...
  {
     flag = 0;
     if (wrong_pattern) {
        break;
     }
     
     while(flag == 0)                             // waiting until we get button push from user
     {
       if (digitalRead(RED_B) == HIGH && debounce_btn())               // if first button is pushed
       {
          Serial.print("Pressed red button turning on red led\n");
          digitalWrite(RED_LED, HIGH);                                          // turn on red led
          your_sequence[i] = 1;                                                 // set value to corresponding led
          flag = 1;                                                             // now that we have input, we can advance the loop
          delay(400);
          while (digitalRead(RED_B) == HIGH) { delay(200); }
          digitalWrite(RED_LED, LOW);              // turn off led
       }
                  // from here on out structure of the code is the same for each led
       if (digitalRead(OR_B) == HIGH && debounce_btn())
       {
          Serial.print("Pressed orange button turning on orange led\n");
          digitalWrite(OR_LED, HIGH);
          your_sequence[i] = 2;
          flag = 1;
          delay(400);
          while (digitalRead(OR_B) == HIGH) { delay(200); }
          digitalWrite(OR_LED, LOW);
       }
    
       if (digitalRead(YEL_B) == HIGH && debounce_btn())
       {
          Serial.print("Pressed yellow button turning on yellow led\n");
          digitalWrite(YEL_LED, HIGH);
          your_sequence[i] = 3;
          flag = 1;
          while (digitalRead(YEL_B) == HIGH) { delay(200); }
          delay(400);
          digitalWrite(YEL_LED, LOW);
       }
    
       if (digitalRead(BLUE_B) == HIGH && debounce_btn())
       {
          Serial.print("Pressed blue button turning on blue led\n");
          digitalWrite(BLUE_LED, HIGH);
          your_sequence[i] = 4;
          flag = 1;
          while (digitalRead(BLUE_B) == HIGH) { delay(200); }
          delay(400);
          digitalWrite(BLUE_LED, LOW);
       }
     }
     if(your_sequence[i] != sequence[i])   // compare your corresponding led value to the generated, if unequal
     {
          Serial.print("incorrect sequence\n");
          wrong_pattern = 1;
          break;                     // wrong button pressed
     }
  }
  digitalWrite(OR_LED, LOW);          // this prevents the last led in user input from staying on
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YEL_LED, LOW);
}

void generate_sequence()              // function generates sequence
{
  randomSeed(millis());
  if (gamemode == 1 || gamemode == 2) {
    if (level == 2) {
      sequence[0] = random(1,5);
      sequence[1] = random(1,5);
    }
    else {
      sequence[level-1] = random(1,5);
    }
  }
  if (gamemode == 3) {
    for (int i = 0; i < level; i++)      // generate a sequence, where length is determined by level variable
    {
      sequence[i] = random(1,5);
    }
  }
}

void get_gamemode()
{
  Serial.print("Red = Normal Mode\nOrange = Fast Mode\nYellow = God Mode\n");
  
  while(1)                             // waiting until we get button push from user
     {
       if (digitalRead(RED_B) == HIGH)               // if red button is pushed,
       {
          digitalWrite(RED_LED, HIGH);
          Serial.print("Playing In Normal Mode\n");
          velocity = 1500;
          gamemode = 1;
          delay(200);
          digitalWrite(RED_LED, LOW);
          break;
       }
       
       if (digitalRead(OR_B) == HIGH)
       {
          digitalWrite(OR_LED, HIGH);
          Serial.print("Playing In Fast Mode\n");
          velocity = 500;
          gamemode = 2;
          delay(200);
          digitalWrite(OR_LED, LOW);
          break;
       }
    
       if (digitalRead(YEL_B) == HIGH)
       {
          digitalWrite(YEL_LED, HIGH);
          Serial.print("Playing In God Mode\n");
          velocity = 500;
          gamemode = 3;
          delay(200);
          digitalWrite(YEL_LED, LOW);
          break;
       }
     }
}

void wrong_sequence()
{
  Serial.print("You made it to level ");
  Serial.print(level-1);
  Serial.print("!\n");
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(RED_LED, HIGH);      // blink led if incorrect sequence is inputted
    delay(250);
    digitalWrite(RED_LED, LOW);
    delay(250);
  }
  level = 2;                          // reset level (i.e number of leds in a sequence)
  gamemode = 0;                       //reset gamemode flag to ask user again
}

void right_sequence()
{
   digitalWrite(RED_LED, LOW);
   digitalWrite(OR_LED, LOW);
   digitalWrite(YEL_LED, LOW);
   digitalWrite(BLUE_LED, LOW);
   delay(250);

  for(int i = 0; i < 3; i++)              // blink all leds when inputted sequence is correct
  {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(OR_LED, HIGH);
    digitalWrite(YEL_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    delay(500);
    digitalWrite(RED_LED, LOW);
    digitalWrite(OR_LED, LOW);
    digitalWrite(YEL_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    delay(500);
  }
  if (level < MAX_LEVEL);
    level++;                                  // increase number of leds in the next sequence
}

int debounce_btn()
{
  int num = 100;
  int i=0;
  while(i<num)
  {
    i++;
  }
  return 1;
}
