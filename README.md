# Gemma Emoji Matrix (Arduino UNO Q + Gemma 4)

Type how you feel in the browser. Gemma 4 detects the emotion (HAPPY, SAD, ANGRY, LOVE, SURPRISED, NEUTRAL)
and the Arduino UNO Q shows the matching emoji on its 8x13 LED matrix.
Built for MLH Hacktoberfest Hack Day — Best Open-Source AI Project / Best Use of Gemma 4.

## AI model
- Model: `gemma-4-26b-a4b-it` (Google Gemma 4, open-weight) via the Gemini API
- Optional local mode: `gemma4:e2b` in Ollama
- Where Gemma is called: `ask_gemini()` in `python/main.py`
- Model terms: https://ai.google.dev/gemma/terms

## How it works
Browser → WebUI brick (port 7000) → Python on the UNO Q asks Gemma 4 for the emotion → Bridge → sketch draws the emoji on the LED matrix.

## Run it
1. Open this app in Arduino App Lab on an UNO Q connected to Wi-Fi.
2. Put your key from https://aistudio.google.com/apikey in `python/api_key.txt`.
3. Click Run and open http://<board-name>.local:7000

## License
MIT — see LICENSE. Gemma weights are covered by Google's Gemma terms.
