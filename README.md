# 😊 Gemma Emoji Matrix

**Type how you feel → Gemma 4 understands the emotion → the Arduino UNO Q shows the emoji on its LED matrix.**

Built for **MLH Hacktoberfest Hack Day**.

## 🏆 Challenges entered
- **Best Open-Source AI Project**: uses Google's open-weight Gemma 4 model, public repository, MIT license
- **Best Use of Gemma 4**: uses Gemma 4 through the Gemini API

## 💡 What problem does it solve?
People don't always say "I am happy" or "I am sad"; they say things like *"I aced my exam!"* or *"my friend ate my lunch"*.
A simple keyword program can't understand sentences like these. Gemma 4 can, so this project turns everyday sentences into a visible emotion on real hardware.
It's a starting point for mood lamps, classroom feedback boards, or accessibility tools for people who find it hard to express feelings.

## 🧠 What does the AI do? (in plain words)
1. You type any sentence in the web page.
2. The program sends the sentence to **Gemma 4** and asks: *"Which emotion is this: HAPPY, SAD, ANGRY, LOVE, SURPRISED or NEUTRAL?"*
3. Gemma 4 reads and understands the sentence and replies with one emotion word.
4. The board draws that emoji on its 8×13 LED matrix.

**Without Gemma 4, the project would only work if you typed the exact emotion word.** Gemma is what lets it understand real sentences.
If Gemma can't be reached, the page clearly shows "keyword fallback" so it's always honest about what made the decision.

## 🤖 AI model
| | |
|---|---|
| Model | `gemma-4-26b-a4b-it` (Google Gemma 4, open-weight) |
| Accessed through | Gemini API |
| Where it is used in the code | `ask_gemini()` in [`python/main.py`](python/main.py) |
| Model terms | [Gemma Terms of Use](https://ai.google.dev/gemma/terms) |
| Optional local mode | `gemma4:e2b` running in Ollama (set `BACKEND = "ollama"`) |

## 🛠️ What we built
- **`sketch/sketch.ino`**: microcontroller code with 6 hand-designed emoji frames for the 8×13 LED matrix, receiving commands through the Arduino Bridge
- **`python/main.py`**: the AI logic: sends the sentence to Gemma 4, reads the answer, picks the emotion, and tells the microcontroller which face to show
- **`assets/index.html`**: a simple web page to type your feeling and see the result
- **`app.yaml`**: Arduino App Lab app settings (uses the WebUI brick)

## 🔄 How it works
Browser (type a sentence)
→ WebUI brick on the UNO Q (port 7000)
→ Python on the UNO Q asks Gemma 4 for the emotion
→ Arduino Bridge
→ Microcontroller draws the emoji on the LED matrix

## 📦 Hardware & dependencies
- **Hardware:** Arduino UNO Q, USB-C cable, Wi-Fi
- **Software:** Arduino App Lab
- **Arduino libraries:** `Arduino_RouterBridge`, `Arduino_LED_Matrix`
- **App Lab brick:** WebUI - HTML
- **Python:** standard library only (`json`, `re`, `urllib`), plus App Lab's built-in `arduino.app_utils`
- **AI:** Gemma 4 via the Gemini API (free key from [Google AI Studio](https://aistudio.google.com/apikey))

## ▶️ How to run it
1. Open **Arduino App Lab**, connect the UNO Q, and connect it to Wi-Fi.
2. Create a new app and add the files from this repository.
3. Create `python/api_key.txt` and paste your Gemini API key into it. **This file is in `.gitignore` and is never uploaded.**
4. Click **Run**.
5. Open `http://<board-name>.local:7000` in a browser on the same Wi-Fi.
6. Type a feeling, for example *"I aced my exam!"*, and watch the board show 😊

## 🎥 Demo
- Video: _add your demo video link here_
- Example results:

| You type | Gemma 4 says | Board shows |
|---|---|---|
| I aced my exam! | HAPPY | 😊 |
| I failed my test | SAD | 😢 |
| my friend ate my lunch | ANGRY | 😠 |
| I love my family | LOVE | ❤️ |
| wow I won?! | SURPRISED | 😮 |

## 📜 License
This project is licensed under the **MIT License**; see [LICENSE](LICENSE).
The Gemma 4 model is provided by Google under the [Gemma Terms of Use](https://ai.google.dev/gemma/terms), not this license.
