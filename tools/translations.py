"""Writes the eleven translation files (rule 66): UTF-16LE with a BOM, "$SEL_<Key><TAB>text" per line, into
dist/.../OBSE/Plugins/ApocryphaMenuFramework/Translations/BetterThirdPersonSelection_<language>.txt. Placeholders (%s) stay as they are.
    python tools/translations.py"""
import os

HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(HERE, "dist", "OblivionRemastered", "Binaries", "Win64", "OBSE", "Plugins", "ApocryphaMenuFramework", "Translations")

KEYS = ["SEL_Intro", "SEL_GroupWhen", "SEL_Enabled", "SEL_ThirdPerson", "SEL_FirstPerson", "SEL_GroupArea", "SEL_Range",
        "SEL_RangeHint", "SEL_Angle", "SEL_AngleHint", "SEL_Reset", "SEL_Selected", "SEL_Nothing", "SEL_StateOff",
        "SEL_StateMenu", "SEL_StateOffThird", "SEL_StateOffFirst", "SEL_StateNoCamera", "SEL_StateCalibrating",
        "SEL_StateNotReady"]

T = {
    "english": ["Use what you are roughly looking at: anything within reach and within the angle below counts, the closest to "
                "where you look first. What the crosshair itself points at always wins.",
                "When", "Wider selection", "In third person", "In first person", "Area", "Reach",
                "How far from your character, in game units (about 70 units to a metre).", "Angle",
                "How far to either side of where the camera looks, in degrees.", "Reset to the defaults", "Selected: %s",
                "Nothing within reach.", "Wider selection is off.", "Paused while a menu is open.", "Off in third person.",
                "Off in first person.", "The camera cannot be read right now.", "Getting ready - walk a few steps.",
                "Waiting for the game."],
    "french": ["Utilisez ce que vous regardez à peu près : tout ce qui est à portée et dans l'angle ci-dessous compte, le plus "
               "proche de votre regard d'abord. Ce que vise le réticule lui-même l'emporte toujours.",
               "Quand", "Sélection élargie", "À la troisième personne", "À la première personne", "Zone", "Portée",
               "Distance à votre personnage, en unités du jeu (environ 70 unités par mètre).", "Angle",
               "Écart de part et d'autre de la direction de la caméra, en degrés.", "Rétablir les valeurs par défaut",
               "Sélection : %s", "Rien à portée.", "La sélection élargie est désactivée.", "En pause tant qu'un menu est ouvert.",
               "Désactivée à la troisième personne.", "Désactivée à la première personne.",
               "La caméra ne peut pas être lue pour l'instant.", "Préparation - faites quelques pas.", "En attente du jeu."],
    "german": ["Benutzt, was Ihr ungefähr anseht: Alles in Reichweite und innerhalb des Winkels unten zählt, das Nächste zu Eurem "
               "Blick zuerst. Worauf das Fadenkreuz selbst zeigt, gewinnt immer.",
               "Wann", "Erweiterte Auswahl", "In der dritten Person", "In der ersten Person", "Bereich", "Reichweite",
               "Wie weit von Eurer Figur, in Spieleinheiten (etwa 70 Einheiten pro Meter).", "Winkel",
               "Wie weit zu jeder Seite der Blickrichtung der Kamera, in Grad.", "Auf die Standardwerte zurücksetzen",
               "Ausgewählt: %s", "Nichts in Reichweite.", "Die erweiterte Auswahl ist aus.", "Pausiert, solange ein Menü offen ist.",
               "Aus in der dritten Person.", "Aus in der ersten Person.", "Die Kamera kann gerade nicht gelesen werden.",
               "Wird vorbereitet - geht ein paar Schritte.", "Warte auf das Spiel."],
    "italian": ["Usa ciò che stai guardando più o meno: conta tutto ciò che è a portata e nell'angolo qui sotto, prima il più "
                "vicino al tuo sguardo. Ciò che il mirino stesso inquadra vince sempre.",
                "Quando", "Selezione ampliata", "In terza persona", "In prima persona", "Area", "Portata",
                "Distanza dal personaggio, in unità di gioco (circa 70 unità per metro).", "Angolo",
                "Ampiezza su ciascun lato della direzione della telecamera, in gradi.", "Ripristina i valori predefiniti",
                "Selezionato: %s", "Niente a portata.", "La selezione ampliata è disattivata.", "In pausa mentre un menu è aperto.",
                "Disattivata in terza persona.", "Disattivata in prima persona.", "Al momento la telecamera non può essere letta.",
                "Preparazione - fai qualche passo.", "In attesa del gioco."],
    "spanish": ["Usa lo que estás mirando más o menos: cuenta todo lo que esté a tu alcance y dentro del ángulo de abajo, primero "
                "lo más cercano a donde miras. Lo que señala la propia mira siempre gana.",
                "Cuándo", "Selección ampliada", "En tercera persona", "En primera persona", "Zona", "Alcance",
                "Distancia a tu personaje, en unidades del juego (unas 70 unidades por metro).", "Ángulo",
                "Amplitud a cada lado de hacia donde mira la cámara, en grados.", "Restablecer los valores predeterminados",
                "Seleccionado: %s", "Nada al alcance.", "La selección ampliada está desactivada.", "En pausa mientras hay un menú abierto.",
                "Desactivada en tercera persona.", "Desactivada en primera persona.", "Ahora no se puede leer la cámara.",
                "Preparando - da unos pasos.", "Esperando al juego."],
    "polish": ["Używaj tego, na co mniej więcej patrzysz: liczy się wszystko w zasięgu i w kącie poniżej, najpierw to najbliżej "
               "kierunku spojrzenia. To, na co wskazuje sam celownik, zawsze wygrywa.",
               "Kiedy", "Szerszy wybór", "W trzeciej osobie", "W pierwszej osobie", "Obszar", "Zasięg",
               "Odległość od postaci w jednostkach gry (około 70 jednostek na metr).", "Kąt",
               "Jak daleko w każdą stronę od kierunku kamery, w stopniach.", "Przywróć ustawienia domyślne", "Wybrano: %s",
               "Nic w zasięgu.", "Szerszy wybór jest wyłączony.", "Wstrzymane, gdy menu jest otwarte.",
               "Wyłączone w trzeciej osobie.", "Wyłączone w pierwszej osobie.", "Nie można teraz odczytać kamery.",
               "Przygotowanie - przejdź kilka kroków.", "Czekam na grę."],
    "czech": ["Používejte to, na co se zhruba díváte: počítá se vše v dosahu a v úhlu níže, nejdřív to nejblíž směru pohledu. "
              "To, na co míří samotný zaměřovač, vždy vyhrává.",
              "Kdy", "Širší výběr", "Z pohledu třetí osoby", "Z pohledu první osoby", "Oblast", "Dosah",
              "Jak daleko od postavy, v herních jednotkách (asi 70 jednotek na metr).", "Úhel",
              "Jak daleko na každou stranu od směru kamery, ve stupních.", "Obnovit výchozí nastavení", "Vybráno: %s",
              "Nic v dosahu.", "Širší výběr je vypnutý.", "Pozastaveno, dokud je otevřená nabídka.",
              "Vypnuto z pohledu třetí osoby.", "Vypnuto z pohledu první osoby.", "Kameru teď nelze přečíst.",
              "Příprava - udělejte pár kroků.", "Čekám na hru."],
    "russian": ["Используйте то, на что вы примерно смотрите: считается всё в пределах досягаемости и угла ниже, сначала ближайшее "
                "к направлению взгляда. То, на что указывает сам прицел, всегда в приоритете.",
                "Когда", "Расширенный выбор", "От третьего лица", "От первого лица", "Область", "Досягаемость",
                "Расстояние от персонажа в игровых единицах (около 70 единиц на метр).", "Угол",
                "Насколько в каждую сторону от направления камеры, в градусах.", "Вернуть значения по умолчанию", "Выбрано: %s",
                "Ничего в пределах досягаемости.", "Расширенный выбор выключен.", "Приостановлено, пока открыто меню.",
                "Выключено от третьего лица.", "Выключено от первого лица.", "Сейчас не удаётся прочитать камеру.",
                "Подготовка - пройдите несколько шагов.", "Ожидание игры."],
    "japanese": ["だいたい見ているものを使えます。届く範囲と下の角度の内側にあるものが対象になり、視線に最も近いものが優先されます。"
                 "照準そのものが指しているものは常に優先されます。",
                 "有効にする場面", "広い選択", "三人称視点", "一人称視点", "範囲", "届く距離",
                 "キャラクターからの距離（ゲーム単位、約 70 単位で 1 メートル）。", "角度",
                 "カメラが向いている方向から左右それぞれの角度（度）。", "初期設定に戻す", "選択中：%s",
                 "届く範囲に何もありません。", "広い選択はオフです。", "メニューを開いている間は一時停止します。",
                 "三人称視点ではオフです。", "一人称視点ではオフです。", "今はカメラを読み取れません。",
                 "準備中です - 数歩歩いてください。", "ゲームを待っています。"],
    "korean": ["대략 바라보는 것을 사용합니다. 닿는 거리와 아래 각도 안에 있는 것이 대상이 되며, 시선에 가장 가까운 것이 먼저입니다. "
               "조준점이 직접 가리키는 것이 항상 우선입니다.",
               "사용 시점", "넓은 선택", "3인칭 시점", "1인칭 시점", "범위", "닿는 거리",
               "캐릭터로부터의 거리, 게임 단위 (약 70단위가 1미터).", "각도",
               "카메라가 보는 방향에서 양쪽으로 각각 몇 도까지인지.", "기본값으로 되돌리기", "선택됨: %s",
               "닿는 거리에 아무것도 없습니다.", "넓은 선택이 꺼져 있습니다.", "메뉴가 열려 있는 동안 일시 정지합니다.",
               "3인칭 시점에서는 꺼져 있습니다.", "1인칭 시점에서는 꺼져 있습니다.", "지금은 카메라를 읽을 수 없습니다.",
               "준비 중입니다 - 몇 걸음 걸어 주세요.", "게임을 기다리는 중입니다."],
    "chinese": ["使用你大致注视的目标：在触及范围和下方角度内的一切都算数，最接近视线的优先。准星本身指向的目标始终优先。",
                "启用时机", "扩大选择", "第三人称", "第一人称", "范围", "触及距离",
                "与角色的距离，以游戏单位计（约 70 单位为 1 米）。", "角度",
                "摄像机朝向两侧各多少度。", "恢复默认设置", "已选择：%s",
                "触及范围内没有目标。", "扩大选择已关闭。", "打开菜单时暂停。",
                "第三人称时关闭。", "第一人称时关闭。", "暂时无法读取摄像机。",
                "准备中 - 请走几步。", "正在等待游戏。"],
}

os.makedirs(OUT, exist_ok=True)
for lang, texts in T.items():
    assert len(texts) == len(KEYS), lang
    for k, t in zip(KEYS, texts):
        assert t.count("%s") == T["english"][KEYS.index(k)].count("%s"), (lang, k)
        assert "%" not in t.replace("%s", ""), (lang, k)
    body = "\r\n".join(f"${k}\t{t}" for k, t in zip(KEYS, texts)) + "\r\n"
    with open(os.path.join(OUT, f"BetterThirdPersonSelection_{lang}.txt"), "wb") as f:
        f.write(b"\xff\xfe" + body.encode("utf-16-le"))
print(f"{len(T)} languages x {len(KEYS)} keys written to {OUT}")
