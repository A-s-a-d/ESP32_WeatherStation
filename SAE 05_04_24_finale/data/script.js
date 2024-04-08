//les ref
const button_On_ref = document.getElementById("button_ON");
const button_Off_ref = document.getElementById("button_OFF");

const button_ok_ville_ref = document.getElementById("buttonID");

const API_ref_ville = document.getElementById("API_ville_ID");
const API_temp_ref = document.getElementById("API_temp_ID");
const API_disc_ref = document.getElementById("Description_id");
const API_img_ref = document.getElementById("id_img");

const API_pres_ref = document.getElementById("API_pres_ID");
const API_hum_ref = document.getElementById("API_hum_ID");


const servo_ref = document.getElementById("servo_range");
const range_value_ref = document.getElementById("angle_range_id");

const date_ref = document.getElementById("id-date");
const day_ref = document.getElementById("id-day");
const time_ref = document.getElementById("id-time");
const submit_ref = document.getElementById("id-submit");

button_Off_ref.addEventListener("click", button_Off_Handler);
button_On_ref.addEventListener("click", button_On_Handler);
servo_ref.addEventListener("input", servo_Handler);

button_ok_ville_ref.addEventListener("click", Button_ville_handler);

function Button_ville_handler(event) {


  const VILLE_value = API_ref_ville.value;
  console.log(API_ref_ville.value);

  const xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/weather?city=" + VILLE_value, true);
  xhttp.send();
  console.log("before JSON");


  xhttp.onreadystatechange = function () {
    console.log("kjdhdskf")
    if (this.readyState == 4 && this.status == 200) {
      console.log("got JSON ***********");
      const jsonville = JSON.parse(this.responseText);
      console.log(jsonville);
      API_temp_ref.textContent = jsonville.main.temp;
      API_hum_ref.textContent = jsonville.main.humidity;
      API_pres_ref.textContent = jsonville.main.pressure;



      API_disc_ref.textContent = jsonville.weather[0].description;
      const img_url = "https://openweathermap.org/img/wn/" + jsonville.weather[0].icon + "@2x.png";
      console.log(img_url);
      API_img_ref.src = img_url;



      console.log(Temp_value);
      API_temp_ref.textContent = Temp_value;
    }
  }
}


function button_On_Handler(event) {
  const xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/action?state=1", true);
  xhttp.send();
  button_On_ref.classList.add("w3-disabled");
  button_Off_ref.classList.remove("w3-disabled");
  console.log("Button On cliqué");
}


function button_Off_Handler(event) {
  const xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/action?state=0", true);
  xhttp.send();
  button_On_ref.classList.remove("w3-disabled");
  button_Off_ref.classList.add("w3-disabled");
  console.log("Button OFF cliqué");

}

function servo_Handler(event) {
  let Servovalue = 0;
  const xhttp = new XMLHttpRequest();
  Servovalue = event.target.value;
  console.log("nouvelle valuer servo : ", Servovalue);
  range_value_ref.textContent = Servovalue

  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.state == 200) {
      Servovalue = this.responseText;
      console.log(Servovalue);
      range_value_ref.textContent = Servovalue;
    }

  }
  xhttp.open("GET", "/servo?servo=" + Servovalue, true);
  xhttp.send();
}








function temperature_Handler() {
  const xhttp = new XMLHttpRequest();
  //
  xhttp.onreadystatechange = function () {
    if

      (this.readyState == 4 && this.status == 200) {
      temperatureValue = this.responseText;
      console.log(temperatureValue);
      //
      gaugeTemperature.refresh(temperatureValue);
    }
  }
  //
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}


function Lux_Handler() {
  const xhttp = new XMLHttpRequest();
  //
  xhttp.onreadystatechange = function () {
    if

      (this.readyState == 4 && this.status == 200) {
      LuxValue = this.responseText;
      console.log(LuxValue);
      //
      gaugeLux.refresh(LuxValue);
    }
  }
  //
  xhttp.open("GET", "/Lux", true);
  xhttp.send();
}

function Hum_Handler() {
  const xhttp = new XMLHttpRequest();
  //
  xhttp.onreadystatechange = function () {
    if

      (this.readyState == 4 && this.status == 200) {
      HumValue = this.responseText;
      console.log(HumValue);
      //
      gaugeHumidity.refresh(HumValue);
    }
  }
  //
  xhttp.open("GET", "/Humidity", true);
  xhttp.send();
}

function Pressure_Handler() {
  const xhttp = new XMLHttpRequest();
  //
  xhttp.onreadystatechange = function () {
    if

      (this.readyState == 4 && this.status == 200) {
      PressureValue = this.responseText;
      console.log(PressureValue);
      //
      gaugePressure.refresh(PressureValue);
    }
  }
  //
  xhttp.open("GET", "/Pressure", true);
  xhttp.send();
}

const gaugeTemperature = new JustGage({
  id: "gauge_temperature",
  value: 0,
  min: -10,
  max: 50,
  decimals: 2,
  title: "",
  label: "°C",
  relativeGaugeSize: true,
  gaugeWidthScale: 0.6,
});
//
const gaugeHumidity = new JustGage({
  id: "gauge_humidity",
  value: getRandomInt(0, 0),
  min: 0,
  max: 100,
  decimals: 2,
  title: "",
  label: "%",
  relativeGaugeSize: true,
  gaugeWidthScale: 0.6,
});
//
const gaugePressure = new JustGage({
  id: "gauge_pressure",
  value: getRandomInt(0, 0),
  min: 0,
  max: 1300,
  decimals: 0,
  title: "",
  label: "hPa",
  relativeGaugeSize: true,
  gaugeWidthScale: 0.6,
});
//
const gaugeLux = new JustGage({
  id: "gauge_lux",
  value: getRandomInt(0, 0),
  min: 0,
  max: 1000,
  decimals: 0,
  title: "",
  label: "Lux",
  relativeGaugeSize: true,
  gaugeWidthScale: 0.6,
});

setInterval(temperature_Handler, 1100);
setInterval(Lux_Handler, 1100);
setInterval(Pressure_Handler, 1100);
setInterval(Hum_Handler, 1100);
setInterval(servo_Handler, 100);

