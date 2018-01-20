/*
    @file     PharmacodeRead.h
    @author   Tomas de Camino Beck (funcostarica.org)
    @license  BSD (see license.txt)

  This a library for reading pharmacodes. it is not to be used with pharmaceuticals
  but as a way of reading paper tags in Arduino. The intention is to use pharmacode
  as a concept, no to strictly implement the method.
  
  POSITION            7   6  5  4  3 2 1 0
  FINE BAR VALUE    128  64 32 16  8 4 2 1
  THICK BAR VALUE   256 128 64 32 16 8 4 2

  The strategy of the code is to pulseIn() to detect the length of a band

  Important
  1. To read, make sure wide bands are 3 times as wide as narrow bands
  2. leave a 3 times narrow band width blank space between bars
  3. A code must have at least one narrow and one wide band
  4. You need an IR track sensor to read bars

  you can find about pharmacodes here:
  http://www.rcelectronica.com/RC/Archivos/Pharma%20Code%20specifications.pdf


  www.funcostarica.org invests time creating these resources. The code is programmed for
  teaching pourposes, not for efficiency, please contribute and share!
  @section  HISTORY v1.0
*/  
/**************************************************************************/

//**************Class PharmaCode Read*************
class pharmacodeRead
{

  public:

    const byte pharmacode[7] = {1, 2, 4, 8, 32, 64, 128}; //pharmacode values


    unsigned long* times; //array that stores bar width (in microseconds)
    int resultLog[10];//log for 10 previous phramacode results
    int nBars; //number of bars to read
    byte sensorPin; //pin with IR sensor (physical code reader)


    double duration, maxV = 0;//pulse duration (band width) and max duration
    int count = 0; //number of bars read in a code
    boolean stopState = false; //true if all n bars have been read
    int result = 0; //stores resulting pharmacode after reading
    byte currentRes = 0; //current result for result log

    //define how many bars to be read on construction
    pharmacodeRead(int nbars, byte pin) {
      nBars = nbars;
      sensorPin = pin;
      times = new unsigned long[nBars];
    }

    //reads bars as they are scanned
    void readCode() {
      duration = pulseIn(sensorPin, HIGH); //detects black bar and width
      if (duration > 0) {
        times[count] = duration;
        maxV = max(maxV, duration);//gets maximum value
        count++;
        duration=0;
      }
      if (count >= nBars ) {
        stopState = true;
        result = 0;
        count=0;
        for (int i = 0; i < nBars; i++) {
          int val = round(times[i] / maxV) + 1; //sets narrow and wide bands
          result += val * pharmacode[i];//get corresponding integer value
        }
        resultLog[currentRes] = result;
        currentRes++;
        if (currentRes > 10) resetLog();
      }
    }

    //resets for new reading
    resetCode() {
      stopState = false;
      count = 0;
      maxV = 0;
      result = 0;
    }

    //clean result log
    resetLog() {
      currentRes = 0;
      for (int i = 0; i < 10; i++)
        resultLog[i] = 0;
    }

    //erases last result in log
    eraseLast() {
      if (currentRes > 0) currentRes--;
      resultLog[currentRes] = 0;
    }

};

