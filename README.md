🤖 Robô Bluetooth com ESP32
Este projeto consiste em um robô controlado remotamente via Bluetooth, utilizando a placa ESP32 como unidade principal de controle. O objetivo é oferecer uma solução acessível e educativa para iniciantes em robótica, automação e comunicação sem fio.

🔧 Características
✅ Controle via aplicativo Bluetooth (Android)

✅ Comunicação sem fio com baixo consumo de energia

✅ Placa ESP32 como microcontrolador principal

✅ Motores DC com ponte H (L298N ou similar)

✅ Bateria recarregável (Li-Ion ou similar)

✅ Estrutura leve e compacta (pode ser impressa em 3D)

📡 Comunicação
A ESP32 é responsável por receber os comandos via Bluetooth (BLE ou Serial) e convertê-los em ações como mover para frente, para trás, virar e parar. Os dados podem ser enviados por um app de smartphone (como o Serial Bluetooth Terminal) ou por um aplicativo personalizado.

🧠 Lógica de Controle
O código embarcado na ESP32 interpreta os comandos recebidos e aciona os pinos responsáveis pelos motores de acordo com a lógica de movimentação. O sistema pode ser facilmente expandido para incluir sensores de obstáculo, controle via Wi-Fi, ou até mesmo funcionalidades autônomas.

🚀 Objetivos educacionais
Este projeto foi desenvolvido com foco no aprendizado de conceitos como:

Programação embarcada (C/C++ para ESP32)

Comunicação Bluetooth

Controle de motores

Integração hardware/software

Noções básicas de eletrônica
