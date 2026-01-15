import pygame


class MusicPlayer:
    def __init__(self, start_listener, timed_listener, stop_listener):
        self.timed_listener = timed_listener
        self.start_listener = start_listener
        self.stop_listener = stop_listener
        self.last_ms = -1  # Track last second

    def playMusic(self, file: str):
        pygame.mixer.init()
        pygame.mixer.music.load(file)
        pygame.mixer.music.play()
        self.start_listener()  # optional start event

        while pygame.mixer.music.get_busy():
            pos_ms = pygame.mixer.music.get_pos()

            if pos_ms != self.last_ms:  # Only trigger on new second
                print(f"\rTime:{(pos_ms/1000):.2f}s", end="")
                self.last_ms = pos_ms
                self.timed_listener(pos_ms)

        self.stop_listener()  # optional stop event
