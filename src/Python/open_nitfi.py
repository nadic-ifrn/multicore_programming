import nibabel as nib
import matplotlib.pyplot as plt

# Load NIfTI file
nifti_file = "dicom_volum_01.png.nii"
img = nib.load(nifti_file)
data = img.get_fdata()

# Visualize a specific slice (e.g., the middle slice)
slice_idx = data.shape[2] // 2
plt.imshow(data[:, :, slice_idx], cmap="gray")
plt.axis("off")
plt.show()
