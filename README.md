# 🎮 Block Buster - A Computer Graphics Project

**Block Buster** is an arcade-style shooting game developed using **C++ with OpenGL and GLUT**. Players control a futuristic ship, shoot bullets, and destroy incoming enemies — including special *Trojan* enemies that require multiple hits and offer bonus points.

## 📸 Screenshots
![Screenshot 2025-05-01 101816](https://github.com/user-attachments/assets/ee800f92-68dd-4c44-b3b4-3c49bd937b3b)


---

## 🚀 Game Features

- **Player Control**: Move left and right, shoot upward bullets.
- **Enemy Types**:
  - 🔴 **Normal Enemies**: Red, die in one hit, +1 point.
  - 🟣 **Trojan Enemies**: Purple, need 2 hits, blink on damage, +3 points.
- 💥 **Explosions**: Colorful visual effects on enemy destruction.
- 📊 **Score Tracking**: Real-time kill count display.
- 🗂️ **In-game Legend**: Shows controls and contributors.

---

## 🎮 Controls

| Key          | Action              |
|--------------|---------------------|
| ⬅️ / ➡️       | Move Ship Left/Right|
| `SPACE`      | Shoot Bullet        |
| `ESC`        | Exit Game           |

---

## 👨‍💻 Contributors

- **Gursamrath Singh** (102217202)
- **Jatin** (102217190)

---

## 🧠 Technical Highlights

- **OpenGL (GLUT)** based rendering
- **Modular code structure** with structs for Bullet, Enemy, and Explosion
- **Real-time animation** using `glutTimerFunc()`
- **Collision detection** between bullets and enemies
- **Random enemy generation**
- **Trojan enemy logic**: blinking and split-hit mechanism

---

## 🔧 Compilation & Execution

### 🔽 Dependencies:
- OpenGL Utility Toolkit (GLUT)

### 🧪 Build Instructions:

#### On Linux:

```bash
g++ BlockBuster.cpp -o BlockBuster -lfreeglut -lopengl32 -lglu32
BlockBuster.exe
