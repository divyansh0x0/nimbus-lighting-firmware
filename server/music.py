import pygame
class MusicPlayer:
    def __init__(self, start_listener, timed_listener, stop_listener):
        self.timed_listener = timed_listener
        self.start_listener = start_listener
        self.stop_listener = stop_listener
        self.last_pos = 0
    def playMusic(self,file:str):
        pygame.mixer.init()
        pygame.mixer.music.load(file)
        pygame.mixer.music.play()

        while pygame.mixer.music.get_busy():
            pos_ms = pygame.mixer.music.get_pos()
            pos_s = pos_ms//1000
            if pos_s > self.last_pos:
                self.timed_listener(pos_s)
                self.last_pos = pos_s
        else:
            self.timed_listener(self.last_pos) # one final call to listener to mark end correctly

def timestamp_listener(pos_ms:int):
    print(pos_ms)