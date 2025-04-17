# 🔐 Base64 — Entendendo a Codificação

Base64 é um esquema de codificação usado para representar dados binários em formato textual (ASCII).
Ele é amplamente utilizado em aplicações como e-mails, URLs, autenticação (como em tokens JWT) e transferência de arquivos pela web.

---

## 📌 Por que usar Base64?

Alguns sistemas foram projetados para manipular apenas **texto simples (ASCII)**.
Como dados binários podem conter caracteres inválidos nesses sistemas, o Base64 entra como solução para converter qualquer tipo de dado em uma **string segura e legível**.

---

## 🔧 Como funciona o Base64?

### 1. **Entrada em blocos de 3 bytes (24 bits)**

- A codificação Base64 trabalha com grupos de **3 bytes** (ou 24 bits).
- Exemplo: `01101101 01100001 01110010` (que representam os caracteres 'm', 'a', 'r').

---

### 2. **Divisão em 4 grupos de 6 bits**

Os 24 bits são divididos em **4 grupos de 6 bits**:

Cada grupo de 6 bits pode representar um valor de 0 a 63.

---

### 3. **Conversão para caracteres da tabela Base64**

Cada valor de 6 bits é mapeado para um caractere específico da tabela:

| Índice | Caractere |
|--------|-----------|
| 0      | A         |
| ...    | ...       |
| 25     | Z         |
| 26     | a         |
| 51     | z         |
| 52     | 0         |
| 61     | 9         |
| 62     | +         |
| 63     | /         |

No exemplo anterior, os valores resultantes apontariam para caracteres como `b`, `W`, `F`, `y`, formando a saída codificada.

---

### 4. **Adição de padding (`=`)**

Se a quantidade de dados não for múltipla de 3, o Base64 adiciona caracteres de preenchimento (`=`) para completar a saída:

| Bytes de Entrada | Saída Base64        |
|------------------|---------------------|
| 1 byte           | 2 caracteres + `==` |
| 2 bytes          | 3 caracteres + `=`  |
| 3 bytes          | 4 caracteres        |

---

## 📤 Exemplo de Codificação

Texto original: Olá

Bytes: [0x4F, 0x6C, 0xC3, 0xA1] → UTF-8

Base64: T2zDqQ==

---

## 📥 Quando usar Base64?

- Quando precisa transmitir dados binários via texto (e-mails, JSON, XML).
- Quando um sistema não aceita caracteres fora do padrão ASCII.
- Para armazenar imagens ou arquivos pequenos diretamente em texto (como em HTML com `data:` URIs).

---

## ⚠️ Atenção

- Base64 **não é criptografia**! Ela apenas codifica dados, não os protege.
- Os dados ainda são legíveis e podem ser facilmente decodificados.

---

## ✅ Curiosidade

O nome "Base64" vem do fato de que ele utiliza **64 caracteres diferentes** para representar os dados (2⁶ = 64).

---

Feito com 💻 para quem quer aprender como os dados viram texto!
