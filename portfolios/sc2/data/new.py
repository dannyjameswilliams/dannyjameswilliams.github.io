import time
import netCDF4 as nc
rain_data = nc.Dataset('precip.V1.0.mon.1981-2010.ltm.nc','r')
precip = rain_data.variables["precip"]
precip_lon = rain_data.variables["lon"]
precip_lat = rain_data.variables["lat"]
precip = precip[0,:,:].T

import matplotlib.pyplot as plt
import numpy as np

plt.imshow(np.sqrt(precip).T, extent=(np.min(precip_lon), np.max(precip_lon), np.min(precip_lat), np.max(precip_lat)), origin='lower')
plt.title('January Sqrt Precipitation Mean (mm) 1981 - 2010')
plt.colorbar(fraction=0.02, pad=0.04)
plt.show()

def bilinear_interpolation(x, y, f_vals, f_coords):
    """
    Function to interpolate on a unit square
    """
    term1 = 1/((f_coords[1]-f_coords[0])*(f_coords[3]-f_coords[2]))
    term2 = [f_coords[1]-x, x-f_coords[0]]
    term3 = np.array(f_vals).reshape(2,2)
    term4 = np.array([[f_coords[3]-y], [y-f_coords[2]]])
    out1 = np.multiply(term1, term2)
    out2 = np.matmul(term3, term4)
    return np.matmul(out1,out2)

def get_corners(x, y, x_grid, y_grid, z):
    """
    Function that returns coordinates and values at the four corners of the grid corresponding to a single observation
    """
    x_order = np.argsort(np.abs(np.subtract(x, x_grid)))
    x1 = np.where(x_grid == x_grid[x_order][0])
    x2 = np.where(x_grid == x_grid[x_order][1])
    
    y_order = np.argsort(np.abs(np.subtract(y, y_grid)))
    y1 = np.where(y_grid == y_grid[y_order][0])
    y2 = np.where(y_grid == y_grid[y_order][1])
    
    x_vals = np.sort(x_grid[np.concatenate((x1,x2),1).flatten() ])
    y_vals = np.sort(y_grid[np.concatenate((y1,y2),1).flatten() ])
    z_vals = np.concatenate((z[x1,y1],z[x1,y2],z[x2,y1],z[x2,y2]),0)
    return x_vals, y_vals, z_vals


if __name__ == "__main__":
    
    grid_size = 10 # degrees
    lon_grid = np.arange(np.min(precip_lon), np.max(precip_lon), grid_size)
    lat_grid = np.arange(np.min(precip_lat), np.max(precip_lat), grid_size)
    
    interp_out = np.empty((lon_grid.size, lat_grid.size)) # allocate matrix to store interpolations
    
    import time
    start = time.time() # save time before loop
    for i in np.arange(lon_grid.size):
            lon = lon_grid[i] # set longitude
            for j in np.arange(lat_grid.size):
                    lat = lat_grid[j] # set latitude
                    
                    # run get_corners function for specified lon and lat
                    lon_vals, lat_vals, precip_vals = get_corners(lon, lat, precip_lon, precip_lat, precip)
                    
                    # filter out any of those points which are outside the range and in the mask
                    if (precip_vals.data<-100).any():
                        interp_out[i, j] = np.nan
                    else:
                        f_coords = np.concatenate((lon_vals, lat_vals))
                        interp_out[i, j] = bilinear_interpolation(lon, lat, precip_vals, f_coords)

    end = time.time() # save end time
    
    
    plt.imshow(np.sqrt(interp_out).T, extent=(np.min(precip_lon),np.max(precip_lon),
                                              np.min(precip_lat),np.max(precip_lat)), 
               origin='lower')
    plt.title('January Interpolated Sqrt Precipitation Mean (mm) 1981 - 2010')
    plt.colorbar(fraction=0.02, pad=0.04)
    plt.show()
    
    print(f"Time taken:", (end-start)/60, "minutes")

    
    def full_interp(lon, lat):
        x, y, z = get_corners(lon, lat, precip_lon, precip_lon, precip)
        print(z)
        #if (z.data < -100).any():
         #   return np.nan
        #else:
        xy = np.concatenate((x, y))
        return float(bilinear_interpolation(lon, lat, z, xy))
        
    grid_size = 10
    lon_grid = np.arange(np.min(precip_lon), np.max(precip_lon), grid_size)
    lat_grid = np.arange(np.min(precip_lat), np.max(precip_lat), grid_size)
    lonlat = np.array([(x, y) for x in lon_grid for y in lat_grid])

    import multiprocessing as mp
    start = time.time()
    with mp.Pool() as pool:
            interp_out = pool.starmap(full_interp, lonlat)
    end = time.time()
    
    print(f"Time taken:", (end-start)/60, "minutes")

    plot_interp = np.array(interp_out).reshape((lon_grid.size, lat_grid.size))
    plt.imshow(np.sqrt(plot_interp).T, extent=(np.min(precip_lon),np.max(precip_lon),
                                                  np.min(precip_lat),np.max(precip_lat)),
                   origin='lower')
    plt.title('January Interpolated Sqrt Precipitation Mean (mm) 1981 - 2010')
    plt.colorbar(fraction=0.02, pad=0.04)
    plt.show()