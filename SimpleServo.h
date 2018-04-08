class SimpleServo
{
public:
  SimpleServo();

  void init();
  void attach(int pin);
  void setPosition(float degree);

private:
  int pin;
};

