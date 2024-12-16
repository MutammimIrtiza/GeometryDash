from PIL import Image
import os

# Path to the input PNG image
input_image_path = "main.png"

# Directory to save the output BMP slices
output_dir = "output_slices"
os.makedirs(output_dir, exist_ok=True)

try:
    # Open the input image
    with Image.open(input_image_path) as img:
        # Check the input image mode and convert if necessary
        if img.mode != "RGB":
            img = img.convert("RGB")

        # Ensure the image dimensions are as expected
        width, height = img.size
        if width != 1600 or height != 200:
            raise ValueError("Input image must be 1600x200 pixels.")

        # Calculate the width of each slice
        slice_width = width // 50  # 32 pixels per slice

        # Create 50 vertical slices
        for i in range(50):
            # Define the cropping box
            left = i * slice_width
            right = left + slice_width
            box = (left, 0, right, height)
            
            # Crop the slice
            slice_img = img.crop(box)
            
            # Save the slice as BMP
            output_path = os.path.join(output_dir, f"slice_{i + 1:02d}.bmp")
            slice_img.save(output_path, format="BMP")

    print(f"Saved 50 BMP slices in the directory: {output_dir}")

except FileNotFoundError:
    print(f"Error: Input image '{input_image_path}' not found.")
except Exception as e:
    print(f"An error occurred: {e}")
