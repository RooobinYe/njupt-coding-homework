import network
import urequests
from machine import Pin, I2C
import ssd1306

weather_translation = {
    "晴": "Sunny",
    "少云": "Partly Cloudy",
    "晴间多云": "Mostly Sunny",
    "多云": "Cloudy",
    "阴": "Overcast",
    "有风": "Windy",
    "平静": "Calm",
    "微风": "Light Breeze",
    "和风": "Gentle Breeze",
    "清风": "Moderate Breeze",
    "强风/劲风": "Fresh Breeze",
    "疾风": "Strong Breeze",
    "大风": "High Wind",
    "烈风": "Gale",
    "风暴": "Storm",
    "狂爆风": "Violent Storm",
    "飓风": "Hurricane",
    "热带风暴": "Tropical Storm",
    "霾": "Haze",
    "中度霾": "Moderate Haze",
    "重度霾": "Heavy Haze",
    "严重霾": "Severe Haze",
    "阵雨": "Showers",
    "雷阵雨": "Thunderstorms",
    "雷阵雨并伴有冰雹": "Thunderstorms with Hail",
    "小雨": "Light Rain",
    "中雨": "Moderate Rain",
    "大雨": "Heavy Rain",
    "暴雨": "Storm",
    "大暴雨": "Severe Storm",
    "特大暴雨": "Extreme Storm",
    "强阵雨": "Strong Showers",
    "强雷阵雨": "Severe Thunderstorms",
    "极端降雨": "Extreme Rainfall",
    "毛毛雨/细雨": "Drizzle",
    "雨": "Rain",
    "小雨-中雨": "Light to Moderate Rain",
    "中雨-大雨": "Moderate to Heavy Rain",
    "大雨-暴雨": "Heavy Rain to Storm",
    "暴雨-大暴雨": "Storm to Severe Storm",
    "大暴雨-特大暴雨": "Severe Storm to Extreme Storm",
    "雨雪天气": "Rain and Snow",
    "雨夹雪": "Sleet",
    "阵雨夹雪": "Showers and Sleet",
    "冻雨": "Freezing Rain",
    "雪": "Snow",
    "阵雪": "Snow Showers",
    "小雪": "Light Snow",
    "中雪": "Moderate Snow",
    "大雪": "Heavy Snow",
    "暴雪": "Blizzard",
    "小雪-中雪": "Light to Moderate Snow",
    "中雪-大雪": "Moderate to Heavy Snow",
    "大雪-暴雪": "Heavy Snow to Blizzard",
    "浮尘": "Dust",
    "扬沙": "Sand",
    "沙尘暴": "Sandstorm",
    "强沙尘暴": "Severe Sandstorm",
    "龙卷风": "Tornado",
    "雾": "Fog",
    "浓雾": "Dense Fog",
    "强浓雾": "Thick Fog",
    "轻雾": "Light Fog",
    "大雾": "Heavy Fog",
    "特强浓雾": "Extreme Fog",
    "热": "Hot",
    "冷": "Cold",
    "未知": "Unknown"
}

def weather_fore(info_type):
    api_url_base = 'https://restapi.amap.com/v3/weather/weatherInfo?city=320113&key=a30216def17cddd80fbdff842ea568f4&extensions=base'
    api_url_all = 'https://restapi.amap.com/v3/weather/weatherInfo?city=320113&key=a30216def17cddd80fbdff842ea568f4&extensions=all'
    
    response_base = urequests.get(api_url_base)
    response_all = urequests.get(api_url_all)

    if response_base.status_code == 200 and response_all.status_code == 200:
        weather_data_base = response_base.json()
        weather_data_all = response_all.json()

        weather_info = {
            'temperature': weather_data_base['lives'][0]['temperature'],
            'humidity': weather_data_base['lives'][0]['humidity'],
            'weather': weather_data_base['lives'][0]['weather'],
            'date': weather_data_all['forecasts'][0]['casts'][1]['date'],
            'forecast': weather_data_all['forecasts'][0]['casts'][1]['dayweather']
        }

        return weather_info.get(info_type, 'Invalid info_type')
    else:
        return 'Failed to get weather data'

def translate_weather(chinese_weather):
    return weather_translation.get(chinese_weather, "Unknown")

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.scan()

if not wlan.isconnected():
    print('connecting to network...')
    wlan.connect('HUAWEI', 'ijuyzlh5z01s')
    while not wlan.isconnected():
        pass

print('network config:', wlan.ifconfig())

temperature = weather_fore('temperature')
humidity = weather_fore('humidity')
weather = weather_fore('weather')

print('Temperature:', temperature)
print('Humidity:', humidity)
print('Weather:', translate_weather(weather))

oled = ssd1306.SSD1306_I2C(128, 64, I2C(scl=Pin(4), sda=Pin(14), freq=400000), addr=0x3C)
oled.fill(0)
oled.text('Temperature: ' + temperature + 'C', 0, 0)
oled.text('Humidity: ' + humidity + '%', 0, 16)
oled.text('Weather: ' + translate_weather(weather), 0, 32)
oled.show()