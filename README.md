# NodeMCU WS2812B controller/webserver
:white_circle: :red_circle: :large_blue_circle: Control your led products (like Adafruit Neopixel) with a nodeMcu (arduino programs).

## Features
- Snake
- Blink
- lignt
- Bright
- Black

## Usage
Connect to your Wi-Fi network. Do POST request on `http://IP_ADDRESS/` with your wanted action and needed datas.

### Snake

#### Description
Light on led one by one.

#### Params
- Color (hex): `color=#ffffff`
- Interval (ms): `interval=150`

#### Example

`curl -X POST 'http://IP_ADDRESS/snake' --data 'color=%23ffffff&interval=150'`


### Blink

#### Description
Reduce and increase brightness.

#### Params
- Time (ms): `time=100`
- Interval (ms): `interval=200`

#### Example

`curl -X POST 'http://IP_ADDRESS/snake' --data 'time=100&interval=200'`


### Light

#### Description
Choose which led to light on.

#### Params
- Color (hex): `color=#ffffff`
- Id: `id=4`

#### Example

`curl -X POST 'http://IP_ADDRESS/light' --data 'color=#ffffff&id=4'`


### Bright

#### Description
Change brightness for all leds.

#### Params
- Bright (0-200): `bright=150`

#### Example

`curl -X POST 'http://IP_ADDRESS/bright' --data 'bright=150'`


### Black

#### Description
Light off all leds.

#### Params


#### Example

`curl -X POST 'http://IP_ADDRESS/black'`