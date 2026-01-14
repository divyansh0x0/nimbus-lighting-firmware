import pygame
import time
import threading
import os

class MusicPlayer:
    def __init__(self, file_path, listener):
        self.file_path = file_path
        self.is_playing = False
        self.listener_active = False
        self.listener = listener
        # Initialize Pygame Mixer
        pygame.mixer.init()

        # Verify file exists
        if not os.path.exists(file_path):
            raise FileNotFoundError(f"Could not find music file: {file_path}")

        pygame.mixer.music.load(self.file_path)

    def _time_listener(self):
        """
        This internal method acts as the event listener.
        It runs on a separate thread to check time every 1s.
        """
        print(f"--- Listener Started for {self.file_path} ---")

        while self.listener_active and pygame.mixer.music.get_busy():
            # get_pos() returns time in milliseconds
            current_ms = pygame.mixer.music.get_pos()

            # Convert to seconds and format
            seconds = int((current_ms / 1000) % 60)
            minutes = int((current_ms / (1000 * 60)) % 60)

            print(f"🎵 Current Time: {minutes:02}:{seconds:02}")
            self.listener(current_ms)

            # Wait exactly 1 second before firing again
            time.sleep(1)

        print("--- Music Ended or Listener Stopped ---")

    def play(self):
        """Starts the music and the 1-second interval listener."""
        if not self.is_playing:
            pygame.mixer.music.play()
            self.is_playing = True
            self.listener_active = True

            # Start the listener in a separate thread so it doesn't block the main program
            listener_thread = threading.Thread(target=self._time_listener, daemon=True)
            listener_thread.start()

    def stop(self):
        """Stops the music and kills the listener."""
        pygame.mixer.music.stop()
        self.is_playing = False
        self.listener_active = False
