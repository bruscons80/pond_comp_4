## Semáforo Inteligente com Arduino Uno e Sensor Ultrassônico

Este documento detalha a montagem e o funcionamento de um protótipo de semáforo inteligente. O sistema utiliza um Arduino Uno para controlar dois cruzamentos (simulados por LEDs) e um sensor ultrassônico para detectar a presença de veículos em uma das vias, otimizando o fluxo.

***OBS: Não participei da aula por conta de uma mudança que tive que fazer pra outra casa. Por isso substitui os 6 leds por 3 leds(vermelho, amarelo e verde) e um RGB (representando um único semáforo), espero que leve em consideração tendo em vista que eu também não tinha os materiais necessários para o modo noturno. O trabalho foi feito apenas por mim.***

---

### 1.0 Lista de Materiais 🛠️

| Quantidade | Componente | Descrição |
| :--- | :--- | :--- |
| 1 | Arduino Uno R3 | O microcontrolador (cérebro) do projeto. |
| 1 | Protoboard (Matriz de Contatos) | Placa para montar o circuito sem solda. |
| 1 | Sensor Ultrassônico HC-SR04 | Usado para detectar a presença (distância) de veículos. |
| 1 | LED RGB (Cátodo Comum) | Simula o Semáforo 1 (Vermelho, Amarelo, Verde). |
| 1 | LED Vermelho (5mm) | Simula a luz vermelha do Semáforo 2. |
| 1 | LED Amarelo (5mm) | Simula a luz amarela do Semáforo 2. |
| 1 | LED Verde (5mm) | Simula a luz verde do Semáforo 2. |
| 5 | Resistores de 220Ω | Para proteger **cada** LED (2 para o RGB, 3 para os LEDs individuais). |
| - | Fios Jumper (Macho-Macho) | Para fazer as conexões na protoboard. |
| 1 | Cabo USB | Para alimentar e programar o Arduino. |

---

### 1.1 Guia de Montagem Física 🔌

A montagem segue 4 etapas principais:

Alimentação da Protoboard:

Conecte o pino 5V do Arduino à barra vermelha (+) da protoboard.

Conecte o pino GND do Arduino à barra azul (-) da protoboard.

Conexão do Sensor Ultrassônico (HC-SR04):

Espete o sensor na protoboard.

Ligue VCC à barra + (5V).

Ligue GND à barra - (GND).

Ligue Trig ao pino digital 7 do Arduino.

Ligue Echo ao pino digital 6 do Arduino.

Conexão do Semáforo 1 (LED RGB):

Espete o LED RGB na protoboard.

Identifique a perna mais longa (Cátodo) e ligue-a diretamente à barra - (GND).

Conecte a perna Vermelha (R): Pino ~9 → Resistor 220Ω → Perna R.

Conecte a perna Verde (G): Pino ~10 → Resistor 220Ω → Perna G.

(A perna Azul (B) não é utilizada).

Conexão do Semáforo 2 (LEDs Individuais):

Para cada um dos três LEDs (Vermelho, Amarelo, Verde), siga este padrão:

Pino do Arduino (3, 4 ou 5) → Resistor 220Ω → Perna Longa (+) do LED.

Perna Curta (-) do LED → Barra - (GND).

---

### 1.2 Mapeamento de Pinos (Pinout) 🗺️

Este mapa detalha qual componente está conectado a qual pino no Arduino Uno.

| Componente | Pino do Componente | Conectado ao Pino do Arduino |
| :--- | :--- | :--- |
| **Alimentação** | Barra Positiva (+) Protoboard | `5V` |
| **Alimentação** | Barra Negativa (-) Protoboard | `GND` |
| | | |
| **Sensor Ultrassônico** | `VCC` | `5V` (Barra +) |
| **Sensor Ultrassônico** | `GND` | `GND` (Barra -) |
| **Sensor Ultrassônico** | `Trig` | **Pino Digital 7** |
| **Sensor Ultrassônico** | `Echo` | **Pino Digital 6** |
| | | |
| **Semáforo 1 (RGB)** | Perna Vermelha (R) | **Pino Digital ~9** (via resistor 220Ω) |
| **Semáforo 1 (RGB)** | Perna Verde (G) | **Pino Digital ~10** (via resistor 220Ω) |
| **Semáforo 1 (RGB)** | Perna Cátodo (longa) | `GND` (Barra -) |
| | | |
| **Semáforo 2 (LED Vermelho)** | Perna Longa (+) | **Pino Digital 5** (via resistor 220Ω) |
| **Semáforo 2 (LED Vermelho)** | Perna Curta (-) | `GND` (Barra -) |
| | | |
| **Semáforo 2 (LED Amarelo)** | Perna Longa (+) | **Pino Digital 4** (via resistor 220Ω) |
| **Semáforo 2 (LED Amarelo)** | Perna Curta (-) | `GND` (Barra -) |
| | | |
| **Semáforo 2 (LED Verde)** | Perna Longa (+) | **Pino Digital 3** (via resistor 220Ω) |
| **Semáforo 2 (LED Verde)** | Perna Curta (-) | `GND` (Barra -) |

---
### 2.0 Funcionalidade do código

O código opera como uma Máquina de Estados que gerencia o fluxo dos semáforos.

Estado 1 (Principal): O Semáforo 1 (RGB) fica Verde e o Semáforo 2 (LEDs) fica Vermelho. O fluxo está na via principal.

Detecção: Enquanto o S1 está verde, o Arduino monitora constantemente o sensor ultrassônico.

Lógica Inteligente: O código verifica a leitura do sensor contra um limite de 30 cm.

Se a distância for > 30 cm (ex: 1003 cm, sem carro): A variável carroEsperando fica false. O Arduino entende que não há tráfego na via secundária e pula o estado de dar a vez ao S2, otimizando o tempo.

Se a distância for < 30 cm (ex: 15 cm, carro detectado): A variável carroEsperando muda para true.

Ciclo de Troca:

O S1 (RGB) muda de Verde para Amarelo e depois Vermelho.

O Arduino verifica: carroEsperando é true?

Sim: O S2 (LEDs) agora fica Verde, permitindo a passagem do carro detectado. Após seu tempo, ele fica Amarelo e depois Vermelho.

Não: O S2 (LEDs) permanece Vermelho o tempo todo.

O ciclo então recomeça, voltando ao Estado 1.

### 2.1 Funcionamento e Integração do Sensor (HC-SR04)

O projeto foi adaptado para usar um Sensor Ultrassônico (HC-SR04) no lugar do sensor LDR, movendo o foco da detecção de luz para a detecção de presença física.

Como o Sensor Funciona:

O sensor HC-SR04 mede a distância usando ondas sônicas. Seu funcionamento é baseado no princípio de um sonar:

Emissão (Trigger): O Arduino envia um pulso elétrico curto (10 microssegundos) para o pino Trig do sensor.

Onda Sônica: O sensor converte esse pulso em um "grito" sônico (inaudível) que viaja pelo ar.

Recepção (Echo): Quando a onda sônica bate em um objeto (como um carro simulado pela mão), ela "ecoa" de volta para o sensor.

Medição de Tempo: O pino Echo do sensor fica em nível ALTO (HIGH) durante o exato tempo que a onda levou para ir e voltar.

Cálculo: O Arduino mede essa duração (em microssegundos) usando a função pulseIn(). Sabendo que a velocidade do som é de ~343 m/s, o código converte esse tempo em distância (em centímetros).

---

### 2.2 Integração com o Sistema:

No projeto, o sensor HC-SR04 é a "inteligência" do Semáforo 2 (a via secundária).

O sensor está posicionado para "olhar" a pista do Semáforo 2.

O código define um limite de DISTANCIA_DETECCAO = 30 cm.

Enquanto o Semáforo 1 está verde, o loop() principal verifica continuamente a distância lida pelo sensor.

Se a distância lida for menor que 30 cm (significando que um objeto está bloqueando o sensor), o sistema entende que "há um carro esperando".

Essa detecção (variável carroEsperando = true) é o gatilho que autoriza o Semáforo 2 a ficar verde no próximo ciclo. Se nenhum carro for detectado (carroEsperando = false), o Semáforo 2 é "pulado" para economizar tempo, mantendo o fluxo na via principal.

---
### Explicação do código em POO

As Classes 

Primeiro, criamos as classes do nosso projeto. No meu caso, um semáforo e um sensor.

class Semaforo { ... };

Isso é a classe de um semáforo.

O que ele tem (Atributos): A classe diz que todo semáforo precisa guardar o número dos seus pinos (pinVermelho, pinAmarelo, pinVerde).

O que ele sabe fazer (Métodos): A classe diz que todo semáforo sabe .verde(), .amarelo() e .vermelho(). A lógica de quais pinos ligar ou desligar está dentro dele.

class SensorUltrassonico { ... };

Isso é a classe de um sensor.

O que ele tem: Os pinos pinTrig e pinEcho.

O que ele sabe fazer: Ele sabe .lerDistancia(). A fórmula de cálculo está dentro dele.

2. Os Objetos 

Agora que temos as classes, podemos construir os objetos reais no início do código:

C++
Semaforo s1(S1_VERMELHO, S1_VERDE);
Semaforo s2(S2_VERMELHO, S2_AMARELO, S2_VERDE);
SensorUltrassonico sensor(PINO_TRIG, PINO_ECHO);
s1: É um objeto Semáforo. Nós o criamos usando a planta Semaforo e dissemos a ele para usar os pinos 9 e 10. (Ele é esperto e sabe que, se só tem 2 pinos, o "amarelo" dele é ligando os dois).

s2: É outro objeto Semáforo. Criamos com a mesma planta, mas dissemos para usar os pinos 5, 4 e 3.

sensor: É o objeto sensor, criado a partir da planta SensorUltrassonico.

Agora você tem três "peças" com "cérebro próprio".

3. O loop() (Dando as Ordens)

O loop() fica muito mais limpo. Em vez de fazer o trabalho sujo, ele agora só dá ordens para os objetos:

Em vez de: controlarSemaforo1('V');

Você diz: s1.verde(); (Que é como dizer: "Objeto s1, faça a sua mágica de ficar verde.")

Em vez de: distancia = lerDistancia();

Você diz: dist = sensor.lerDistancia(); (Que é como dizer: "Objeto sensor, me diga qual é a distância agora.")

Resumo da Vantagem

O loop() não precisa saber como um semáforo fica verde (quais pinos ligar/desligar). Ele só dá a ordem. Toda a responsabilidade e a lógica estão guardadas dentro do objeto s1.

Isso torna o código:

Mais organizado: Cada classe cuida da sua própria lógica.

Mais legível: O loop() lê como uma lista de comandos simples.

Reutilizável: Se você quisesse 10 semáforos, era só criar s3, s4, etc.

<br>
<div align="center">
<sub>Projeto</sub>
</div>
<br>
<div align="center">
<img width=100% height=100% alt="VGND" src="Fotos/Videos/WhatsApp Image 2025-11-13 at 8.39.14 PM.jpeg"/>
</div>
<br>
<div align="center">
</div>
<br>


<br>
<div align="center">
<sub>Projeto</sub>
</div>
<br>
<div align="center">
<img width=100% height=100% alt="VGND" src="Fotos/Videos/Fotos/Videos/WhatsApp Video 2025-11-13 at 8.39.32 PM.mp4"/>
</div>
<br>
<div align="center">
</div>
<br>