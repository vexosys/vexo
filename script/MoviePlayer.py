from ffpyplayer.player import MediaPlayer
import cv2

# What to do before the player starts.
print("Simple Windowed Video Player with OpenCV and FFPyPlayer.")
print("There might be some audio errors, forgive me for that.")

# Define the Video directory.
video_path="sample.mp4"

# The PlayVideo function.
def PlayVideo(video_path):
    # Define the stuff that will happen.
    video=cv2.VideoCapture(video_path)
    player = MediaPlayer(video_path)
    # Loop the content while the video plays.
    while True:
        grabbed, frame=video.read()
        audio_frame, val = player.get_frame()
        if not grabbed:
            # Do something after the video ends.
            print()
            break
        if cv2.waitKey(28) & 0xFF == ord("q"):
            break
        cv2.namedWindow('Movie Player with Audio | Python 3.0, OpenCV and FFPyPlayer. (Press Q to Quit)', flags=cv2.WINDOW_GUI_NORMAL) # Define the GUI mode.
        cv2.resizeWindow("Movie Player with Audio | Python 3.0, OpenCV and FFPyPlayer. (Press Q to Quit)", 768, 432) # Define the Window Resolution.
        cv2.imshow("Movie Player with Audio | Python 3.0, OpenCV and FFPyPlayer. (Press Q to Quit)", frame) # Show the frame.
        if val != 'eof' and audio_frame is not None: # Attempt to sync the movie with the audio.
            img, t = audio_frame
    # Finish the task if the video ends.
    video.release()
    cv2.destroyAllWindows()

# Start the PlayVideo function with the path.
PlayVideo(video_path)
