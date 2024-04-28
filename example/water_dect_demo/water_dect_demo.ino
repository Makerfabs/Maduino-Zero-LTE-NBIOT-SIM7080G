#define DEBUG true
#define LTE_RESET_PIN -1
#define LTE_PWRKEY_PIN 5

#define SENSOR_PIN 9

#define EMAIL_SERVER "smtp.163.com"
#define EMAIL_ID "EMAIL_ID"
#define EMAIL_POP_PWD "EMAIL_POP_PWD"
#define FROM_NAME "Sensor Alarm"
#define FROM_ADD "XXXXXXX@163.com"
#define TO_NAME "Myself"
#define TO_ADD "XXXXXXX@163.com"

int sensor_flag = 0;

void setup()
{
    SerialUSB.begin(115200);
    Serial1.begin(115200);

    pinMode(LTE_RESET_PIN, OUTPUT);
    pinMode(LTE_PWRKEY_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT);

    digitalWrite(LTE_RESET_PIN, LOW);
    digitalWrite(LTE_RESET_PIN, LOW);
    delay(100);
    digitalWrite(LTE_PWRKEY_PIN, HIGH);
    delay(3000);
    digitalWrite(LTE_PWRKEY_PIN, LOW);

    SerialUSB.println("Maduino Zero LTE CAT-M1 CAT1 NB1/NB2 Test Start!");
    SerialUSB.println("Wait a few minutes for LTE star");
    delay(3000);

    // AT init
    sendData("AT", 1000, DEBUG);
    sendData("AT+CPIN?", 1000, DEBUG);
    sendData("AT+CCID", 1000, DEBUG);
    sendData("AT+CSQ", 1000, DEBUG);
    sendData("AT+COPS?", 1000, DEBUG);
    sendData("AT+CGATT?", 1000, DEBUG);
    sendData("AT+CNACT=0,1", 1000, DEBUG);
    sendData("AT+CNACT?", 1000, DEBUG);
    sendData("AT+CPSI?", 1000, DEBUG);
}

void loop()
{
    sensor_read();
    delay(100);
    email_task();

    // Delay 5min
    for (int i = 0; i < 5; i++)
        delay(60000L);
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

void email_task()
{
    String temp = "";
    sendData("AT+EMAILCID=0", 1000, DEBUG);
    sendData("AT+EMAILTO=30", 1000, DEBUG);

    // Your mail server port
    temp = temp + "AT+SMTPSRV=\"" + EMAIL_SERVER + "\",25";
    sendData(temp, 1000, DEBUG);
    // Your email id and smtp password not email password
    temp = "";
    temp = temp + "AT+SMTPAUTH=1,\"" + EMAIL_ID + "\",\"" + EMAIL_POP_PWD + "\"";
    sendData(temp, 1000, DEBUG);
    // Sender Address
    temp = "";
    temp = temp + "AT+SMTPFROM=\"" + FROM_ADD + "\",\"" + FROM_NAME + "\"";
    sendData(temp, 1000, DEBUG);
    // Recipient Address
    temp = "";
    temp = temp + "AT+SMTPRCPT=0,0,\"" + TO_ADD + "\",\"" + TO_NAME + "\"";
    sendData(temp, 1000, DEBUG);

    if (sensor_flag == 1)
    {
        // Subject
        sendData("AT+SMTPSUB=\"Water full\"", 1000, DEBUG);
        // Set body length
        sendData("AT+SMTPBODY=12", 1000, DEBUG);
        // Set body
        sendData("Water full.\n\r", 1000, DEBUG);
    }
    else
    {
        // Subject
        sendData("AT+SMTPSUB=\"Water empty\"", 1000, DEBUG);
        // Set body length
        sendData("AT+SMTPBODY=13", 1000, DEBUG);
        // Set body
        sendData("Water empty.\n\r", 1000, DEBUG);
    }

    // Send
    sendData("AT+SMTPSEND", 1000, DEBUG);
}

void sensor_read()
{
    if (digitalRead(SENSOR_PIN) == LOW)
    {
        sensor_flag = 1;
        SerialUSB.println("Low, have water");
    }

    else
    {
        sensor_flag = 0;
        SerialUSB.println("High, no water");
    }
}