import matplotlib.pyplot as plt
import sys, os
from math import ceil, sqrt, floor
from glob import glob
import numpy as np
import argparse


def is_correct_image(path: str):
    try:
        plt.imread(path)
        return True
    except Exception as e:
        return False


def visu(img_paths):
    # img_paths = ["C:/codes/CPP/Donnees_Multimedia/Image/output/TP1_exo1*.pgm"]
    if isinstance(img_paths, str):
        img_paths = [img_paths]
    img_paths = np.array([glob(path, recursive=True) for path in img_paths]).flatten()
    img_paths = [path for path in img_paths if is_correct_image(path)]
    if len(img_paths) == 0:
        print("No image found, sorry...")
        exit(0)

    width = floor(sqrt(len(img_paths)))
    height = width
    if width * height < len(img_paths):
        width += 1
    if width * height < len(img_paths):
        height += 1
    fig, axes = plt.subplots(height, width, squeeze=False)
    i_current = 0
    j_current = 0
    cumulative = None
    for img_path in img_paths:
        img = plt.imread(img_path)
        axes[j_current, i_current].imshow(img, cmap="gray")
        axes[j_current, i_current].set_title(os.path.basename(img_path))

        if cumulative is None:
            cumulative = img / len(img_paths)
        else:
            cumulative += img / len(img_paths)

        i_current += 1
        if i_current >= width:
            i_current = 0
            j_current += 1

        print(f"{np.count_nonzero(img)} pixels blancs et {img.shape[0]*img.shape[1] - np.count_nonzero(img)} pixels noirs dans l'image", img_path)
    plt.tight_layout()
    plt.show()

    # plt.imshow(cumulative, 'gray')
    # plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='''Script de visualisation d'images PPM''')
    parser.add_argument("path", default="*.ppm", nargs="*",
                        help="Chemin vers les images à afficher")
    args = vars(parser.parse_args())
    path = args["path"]
    visu(path)