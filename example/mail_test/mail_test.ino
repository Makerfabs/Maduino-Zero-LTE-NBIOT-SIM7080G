#define DEBUG true
#define LTE_RESET_PIN -1
#define LTE_PWRKEY_PIN 5

void setup()
{

    SerialUSB.begin(115200);
    Serial1.begin(115200);
    delay(1000);

    pinMode(LTE_RESET_PIN, OUTPUT);
    digitalWrite(LTE_RESET_PIN, LOW);
    pinMode(LTE_PWRKEY_PIN, OUTPUT);
    digitalWrite(LTE_RESET_PIN, LOW);
    delay(100);
    digitalWrite(LTE_PWRKEY_PIN, HIGH);
    delay(3000);
    digitalWrite(LTE_PWRKEY_PIN, LOW);

    SerialUSB.println("Maduino Zero LTE CAT-M1 CAT1 NB1/NB2 Test Start!");
    SerialUSB.println("Wait a few minutes for LTE star");
    delay(3000);

    sendData("AT", 1000, DEBUG);
    sendData("AT+CPIN?", 1000, DEBUG);
    sendData("AT+CCID", 1000, DEBUG);
    sendData("AT+CSQ", 1000, DEBUG);
    sendData("AT+COPS?", 1000, DEBUG);
    sendData("AT+CGATT?", 1000, DEBUG);
    sendData("AT+CNACT=0,1", 1000, DEBUG);
    sendData("AT+CNACT?", 1000, DEBUG);

    // Synchronize time 同步时间
    sendData("AT+CNTPCID?", 1000, DEBUG);
    sendData("AT+CNTP=\"ntp1.aliyun.com\",32", 1000, DEBUG);
    sendData("AT+CNTP", 1000, DEBUG);
    sendData("AT+CCLK?", 1000, DEBUG);

    sendData("AT+SIMCOMATI", 1000, DEBUG);
    sendData("AT+GMR", 1000, DEBUG);
    sendData("AT+CPSI?", 1000, DEBUG);

    sendData("AT+EMAILCID=0", 1000, DEBUG);
    sendData("AT+EMAILTO=30", 1000, DEBUG);
    // Your mail server port
    sendData("AT+SMTPSRV=\"smtp.163.com\",25", 1000, DEBUG);
    // Your email id and smtp password not email password
    sendData("AT+SMTPAUTH=1,\"EMAIL_ID\",\"EMAIL_SMTP_PWD\"", 1000, DEBUG);

    // Sender Address
    sendData("AT+SMTPFROM=\"15694500215@163.com\",\"xbw\"", 1000, DEBUG);
    // Recipient Address
    sendData("AT+SMTPRCPT=0,0,\"15694500215@163.com\",\"xbw\"", 1000, DEBUG);
    // Subject
    sendData("AT+SMTPSUB=\"Test\"", 1000, DEBUG);
    // Set body length
    sendData("AT+SMTPBODY=19", 1000, DEBUG);
    // Set body
    sendData("This is a new Email 1234567890\n\r", 1000, DEBUG);
    // Send
    sendData("AT+SMTPSEND", 1000, DEBUG);
}

void loop()
{
    while (Serial1.available() > 0)
    {
        SerialUSB.write(Serial1.read());
        yield();
    }
    while (SerialUSB.available() > 0)
    {
        Serial1.write(SerialUSB.read());
        yield();
    }
}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    Serial1.println(command);
    long int time = millis();
    while ((time + timeout) > millis())
    {
        while (Serial1.available())
        {
            char c = Serial1.read();
            response += c;
        }
    }
    if (debug)
    {
        SerialUSB.print(response);
    }
    return response;
}
