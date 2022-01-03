

const int bleedAirPin = 22;
const int pitotHeatMagPin = 23;
const int laserArmMagPin = 24;
const int canopyMagPin = 25;
const int landingGearSolPin = 26;
const int launchBarMagPin = 27;
const int hookBypassMagPin = 28;
const int fuelDumpMagPin = 29;
const int APUMagPin = 30;
const int engineCrankMagPin = 31;

const int delayTime = 500;

void setup() {

  pinMode(bleedAirPin, OUTPUT);
  pinMode(pitotHeatMagPin, OUTPUT);
  pinMode(laserArmMagPin, OUTPUT);
  pinMode(canopyMagPin, OUTPUT);
  pinMode(landingGearSolPin, OUTPUT);
  pinMode(launchBarMagPin, OUTPUT);
  pinMode(hookBypassMagPin, OUTPUT);
  pinMode(fuelDumpMagPin, OUTPUT);
  pinMode(APUMagPin, OUTPUT);
  pinMode(engineCrankMagPin, OUTPUT);
}

void loop() {


  digitalWrite(bleedAirPin, HIGH);
  digitalWrite(pitotHeatMagPin, HIGH);
  digitalWrite(laserArmMagPin, HIGH);
  digitalWrite(canopyMagPin, HIGH);
  digitalWrite(landingGearSolPin, HIGH);
  digitalWrite(launchBarMagPin, HIGH);
  digitalWrite(hookBypassMagPin, HIGH);
  digitalWrite(fuelDumpMagPin, HIGH);
  digitalWrite(APUMagPin, HIGH);
  digitalWrite(engineCrankMagPin, HIGH);


  delay(delayTime);
  
  digitalWrite(bleedAirPin, LOW);
  digitalWrite(pitotHeatMagPin, LOW);
  digitalWrite(laserArmMagPin, LOW);
  digitalWrite(canopyMagPin, LOW);
  digitalWrite(landingGearSolPin, LOW);
  digitalWrite(launchBarMagPin, LOW);
  digitalWrite(hookBypassMagPin, LOW);
  digitalWrite(fuelDumpMagPin, LOW);
  digitalWrite(APUMagPin, LOW);
  digitalWrite(engineCrankMagPin, LOW);

  delay(delayTime);

}
