import matplotlib.pyplot as plt

def plot_univariate_data(filenames, n, x_label="Number of Intersections", y_label="Compute Time (s)", title="Compute Time vs Number of Intersections (n = {})", ax=None):
    """
    Plot univariate data from one or more files where each line contains an x,y pair separated by a space.
    
    Args:
        filenames (str or list): Path(s) to the file(s) containing the data
        n (int): Value to be included in the title
        x_label (str): Label for the x-axis
        y_label (str): Label for the y-axis
        title (str): Title of the plot (will be formatted with n)
        ax (matplotlib.axes.Axes, optional): Axes to plot on. If None, creates a new figure.
    """
    # Convert single filename to list for consistent handling
    if isinstance(filenames, str):
        filenames = [filenames]
    
    # Create new figure if no axes provided
    if ax is None:
        plt.figure(figsize=(10, 6))
        ax = plt.gca()
    
    # Different colors and markers for each dataset
    colors = ['b', 'r', 'g', 'c', 'm', 'y']
    markers = ['o', 's', '^', 'D', 'v', '<']
    
    for i, filename in enumerate(filenames):
        # Read data from file
        x_values = []
        y_values = []
        
        with open(filename, 'r') as f:
            for line in f:
                x, y = map(float, line.strip().split())
                x_values.append(x)
                y_values.append(y)
        
        # Plot with different color and marker for each dataset
        color = colors[i % len(colors)]
        marker = markers[i % len(markers)]
        ax.plot(x_values, y_values, f'{color}{marker}-', 
                linewidth=2, markersize=6, 
                label=f'Dataset {i+1}')
    
    # Add labels and title
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.set_title(title.format(n))
    
    # Add grid for better readability
    ax.grid(True, linestyle='--', alpha=0.7)
    
    # Add legend
    ax.legend()

def plot_all_datasets():
    """Create a 2x2 grid of plots for all datasets."""
    # Create figure with 2x2 subplots
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    fig.suptitle('Compute Time Analysis for Different Values of n = # of segments', fontsize=16)
    
    # Plot each dataset in its own subplot
    # plot_univariate_data(["data_361_1.txt", "data_361_2.txt", "data_361_3.txt"], n=361, ax=axes[0,0])
    # plot_univariate_data(["data_1000_1.txt", "data_1000_2.txt", "data_1000_3.txt"], n=1000, ax=axes[0,1])
    plot_univariate_data(["data_3612_1.txt", "data_3612_2.txt", "data_3612_3.txt"], n=3612, ax=axes[0,0])
    plot_univariate_data(["data_10000_1.txt", "data_10000_2.txt", "data_10000_3.txt"], n=10000, ax=axes[0,1])
    plot_univariate_data(["data_31623_1.txt", "data_31623_2.txt", "data_31623_3.txt"], n=31623, ax=axes[1,0])
    plot_univariate_data(["data_100000_1.txt", "data_100000_2.txt", "data_100000_3.txt"], n=100000, ax=axes[1,1])
    
    # Adjust layout to prevent overlap
    plt.tight_layout()
    
    # Show the plot
    plt.show()

# Example usage:
if __name__ == "__main__":
    plot_all_datasets()
