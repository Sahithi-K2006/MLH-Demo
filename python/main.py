# Gemma Emoji Matrix - Linux side (Arduino UNO Q, App Lab)
# You type a sentence in the browser -> Gemma 4 decides the emotion ->
# the MCU draws that emoji on the LED matrix.
# License: MIT

import json
import re
import urllib.request
from pathlib import Path

from arduino.app_utils import App, Bridge
from arduino.app_bricks.web_ui import WebUI

# ------------------ SETTINGS ------------------
BACKEND = "gemini"                         # "gemini" = Gemma 4 via the Gemini API (use this)
                                           # "ollama" = Gemma 4 on your laptop (optional, advanced)
GEMINI_MODEL = "gemma-4-26b-a4b-it"        # Gemma 4 on the Gemini API
OLLAMA_URL = "http://192.168.1.50:11434"   # only used if BACKEND = "ollama"
OLLAMA_MODEL = "gemma4:e2b"                # only used if BACKEND = "ollama"
# -----------------------------------------------

EMOTIONS = ["HAPPY", "SAD", "ANGRY", "LOVE", "SURPRISED", "NEUTRAL"]
EMOJI = {"HAPPY": "😊", "SAD": "😢", "ANGRY": "😠", "LOVE": "❤️", "SURPRISED": "😮", "NEUTRAL": "😐", "HeartBreak": "💔"}

PROMPT = (
    "Classify the emotion of the message below. "
    "Reply with exactly ONE word from this list and nothing else: "
    "HAPPY, SAD, ANGRY, LOVE, SURPRISED, NEUTRAL.\n\n"
    "Message: {text}"
)

KEY_FILE = Path(__file__).resolve().parent / "api_key.txt"


def post_json(url, payload, headers=None, timeout=120):
    data = json.dumps(payload).encode("utf-8")
    req = urllib.request.Request(url, data=data, method="POST",
                                 headers={"Content-Type": "application/json", **(headers or {})})
    with urllib.request.urlopen(req, timeout=timeout) as resp:
        return json.loads(resp.read().decode("utf-8"))


def ask_gemini(text):
    key = KEY_FILE.read_text().strip() if KEY_FILE.exists() else ""
    if not key:
        raise RuntimeError("No API key: create python/api_key.txt")
    url = f"https://generativelanguage.googleapis.com/v1beta/models/{GEMINI_MODEL}:generateContent"
    result = post_json(url, {"contents": [{"parts": [{"text": PROMPT.format(text=text)}]}]},
                       headers={"x-goog-api-key": key})
    parts = result["candidates"][0]["content"]["parts"]
    return " ".join(p.get("text", "") for p in parts if not p.get("thought"))


def ask_ollama(text):
    result = post_json(f"{OLLAMA_URL}/api/chat", {
        "model": OLLAMA_MODEL,
        "messages": [{"role": "user", "content": PROMPT.format(text=text)}],
        "stream": False,
        "options": {"temperature": 0},
    })
    return result["message"]["content"]


def pick_emotion(reply):
    """Find the first allowed emotion word in Gemma's reply."""
    words = re.findall(r"[A-Z]+", (reply or "").upper())
    for w in words:
        if w in EMOTIONS:
            return w
    return "NEUTRAL"


def keyword_fallback(text):
    """Only used if Gemma can't be reached, so the demo never freezes."""
    words = re.findall(r"[A-Z]+", text.upper())
    for w in words:
        if w in EMOTIONS:
            return w
    return "NEUTRAL"


def analyze(text: str = ""):
    text = text.strip()
    if not text:
        return {"error": "Type something first."}
    model = GEMINI_MODEL if BACKEND == "gemini" else OLLAMA_MODEL
    try:
        reply = ask_gemini(text) if BACKEND == "gemini" else ask_ollama(text)
        emotion = pick_emotion(reply)
        source = f"Gemma 4 ({model}, {BACKEND})"
    except Exception as e:
        print("Gemma error:", e, flush=True)
        emotion = keyword_fallback(text)
        source = f"keyword fallback - Gemma error: {e}"
    Bridge.call("show_emotion", EMOTIONS.index(emotion))
    print(f"'{text}' -> {emotion} [{source}]", flush=True)
    return {"text": text, "emotion": emotion, "emoji": EMOJI[emotion], "source": source}


print(f"Gemma Emoji Matrix starting. Backend={BACKEND}", flush=True)
web = WebUI()
web.expose_api("GET", "/analyze", analyze)
App.run()
