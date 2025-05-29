from matplotlib import pyplot as plt
import numpy as np
import sys

def write_segments_to_file(segments, filename):
    """
    Write line segments to a file.
    Each line will contain 4 floats: x1 y1 x2 y2
    
    Args:
        segments (list): List of numpy arrays, each representing a line segment
        filename (str): Name of the file to write to
    """
    with open(filename, 'w') as f:
        for segment in segments:
            x1, y1 = segment[0]
            x2, y2 = segment[1]
            f.write(f"{x1:.6f} {y1:.6f} {x2:.6f} {y2:.6f}\n")

def generate_random_segments(num_segments, max_len):
    """
    Generate random line segments within the unit square [0,1]x[0,1].
    Each segment will have a length between 0.5*max_len and max_len.
    Segments that would be too short after boundary adjustment are rejected.
    
    Args:
        num_segments (int): Number of segments to generate
        max_len (float): Maximum length of segments (minimum will be 0.5*max_len)
    
    Returns:
        list: List of numpy arrays, each representing a line segment
    """
    segments = []
    min_len = 0.5 * max_len
    
    while len(segments) < num_segments:
        # Generate random start point
        x1 = np.random.random()
        y1 = np.random.random()
        
        # Generate random angle
        angle = np.random.random() * 2 * np.pi
        
        # Generate random length between min_len and max_len
        length = min_len + np.random.random() * (max_len - min_len)
        
        # Calculate end point
        x2 = x1 + length * np.cos(angle)
        y2 = y1 + length * np.sin(angle)
        
        # If end point is outside unit square, adjust it
        if x2 < 0 or x2 > 1 or y2 < 0 or y2 > 1:
            # Calculate intersection with unit square boundary
            t = 1.0
            if x2 < 0:
                t = min(t, -x1 / (x2 - x1))
            if x2 > 1:
                t = min(t, (1 - x1) / (x2 - x1))
            if y2 < 0:
                t = min(t, -y1 / (y2 - y1))
            if y2 > 1:
                t = min(t, (1 - y1) / (y2 - y1))
            
            x2 = x1 + t * (x2 - x1)
            y2 = y1 + t * (y2 - y1)
            
            # Calculate actual length after adjustment
            actual_length = np.sqrt((x2 - x1)**2 + (y2 - y1)**2)
            
            # Reject if too short
            if actual_length < min_len:
                continue
        
        segment = np.array([[x1, y1], [x2, y2]])
        segments.append(segment)
    
    return segments

def read_segments_from_file(filename):
    """
    Read line segments from a file.
    Each line should contain 4 floats: x1 y1 x2 y2
    Returns a list of numpy arrays, where each array represents a line segment.
    """
    segments = []
    with open(filename, 'r') as f:
        for line in f:
            # Split the line and convert to floats
            coords = list(map(float, line.strip().split()))
            if len(coords) == 4:
                # Create a 2x2 array for the segment
                segment = np.array([[coords[0], coords[1]], [coords[2], coords[3]]])
                segments.append(segment)
    return segments

def plot_segments(segments):
    plt.figure(figsize=(10, 10))
    for segment in segments:
        plt.plot(segment[:, 0], segment[:, 1], 'r-')
    plt.show()

def main():
    args = sys.argv[1:]

    if len(args) == 1:
        segments = read_segments_from_file(filename=args[0])
        plot_segments(segments=segments)
 
if __name__ == '__main__':
    main()




