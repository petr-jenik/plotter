; KISSlicer - FREE
; Linux
; version 1.5 Release Linux32
; Built: Aug  4 2016, 11:36:46
; Running on 2 cores
;
; Saved: Sun May 21 08:24:00 2017
; 'cat.gcode'
;
; *** Printer Settings ***
;
; printer_name = sample printer
; bed_STL_filename = 
; extension = gcode
; cost_per_hour = 0
; g_code_prefix = 3B205B6D6D5D206D6F64650A4732310A3B206162736F
;     6C757465206D6F64650A4739300A
; > Decoded
; >   ; [mm] mode
; >   G21
; >   ; absolute mode
; >   G90
; g_code_warm = 3B2053656C6563742065787472756465722C207761726D
;     2C2070757267650A0A3B204266422D7374796C650A4D3C4558542B31
;     3E303420533C54454D503E0A4D3534320A4D35353C4558542B313E20
;     50333230303020533930300A4D3534330A0A3B2035442D7374796C65
;     0A543C4558542B303E0A4D31303920533C54454D503E0A
; > Decoded
; >   ; Select extruder, warm, purge
; >   ; BfB-style
; >   M<EXT+1>04 S<TEMP>
; >   M542
; >   M55<EXT+1> P32000 S900
; >   M543
; >   ; 5D-style
; >   T<EXT+0>
; >   M109 S<TEMP>
; g_code_same_warm = NULL
; > Decoded
; g_code_same_cool = NULL
; > Decoded
; g_code_cool = 3B2047756172616E746565642073616D65206578747275
;     6465722C206275742061626F757420746F20646573656C6563742C20
;     6D617962652072657472616374206265666F726520636F6F6C696E67
;     20646F776E0A0A3B204266422D7374796C650A4D3C4558542B313E30
;     3420533C54454D503E0A0A3B2035442D7374796C650A4D3130342053
;     3C54454D503E0A
; > Decoded
; >   ; Guaranteed same extruder, but about to deselect, maybe
; >    retract before cooling down
; >   ; BfB-style
; >   M<EXT+1>04 S<TEMP>
; >   ; 5D-style
; >   M104 S<TEMP>
; g_code_N_layers = 3B204D617962652072652D686F6D65205820262059
;     3F
; > Decoded
; >   ; Maybe re-home X & Y?
; g_code_postfix = 3B20416C6C207573656420657874727564657273206
;     1726520616C72656164792027436F6F6C65642720746F20300A
; > Decoded
; >   ; All used extruders are already 'Cooled' to 0
; post_process = NULL
; > Decoded
; every_N_layers = 0
; num_extruders = 1
; firmware_type = 1
; add_comments = 1
; fan_on = M106
; fan_off = M107
; fan_pwm = 1
; add_m101_g10 = 0
; z_speed_mm_per_s = 3.5
; z_settle_mm = 0.25
; bed_size_x_mm = 100
; bed_size_y_mm = 100
; bed_size_z_mm = 100
; ext_radius = 0
; bed_offset_x_mm = 0
; bed_offset_y_mm = 0
; bed_offset_z_mm = 0
; bed_roughness_mm = 0.25
; round_bed = 0
; travel_speed_mm_per_s = 500
; rim_speed_mm_per_s = 250
; first_layer_speed_mm_per_s = 10
; dmax_per_layer_mm_per_s = 50
; xy_accel_mm_per_s_per_s = 1500
; xy_steps_per_mm = 150
; lo_speed_perim_mm_per_s = 5
; lo_speed_loops_mm_per_s = 20
; lo_speed_solid_mm_per_s = 15
; lo_speed_sparse_mm_per_s = 30
; hi_speed_perim_mm_per_s = 15
; hi_speed_loops_mm_per_s = 65
; hi_speed_solid_mm_per_s = 60
; hi_speed_sparse_mm_per_s = 75
; ext_gain_1 = 1
; ext_material_1 = 0
; ext_axis_1 = 0
; ext_gain_2 = 1
; ext_material_2 = 0
; ext_axis_2 = 0
; ext_Xoff_2 = 0
; ext_Yoff_2 = 0
; ext_gain_3 = 1
; ext_material_3 = 0
; ext_axis_3 = 0
; ext_Xoff_3 = 0
; ext_Yoff_3 = 0
; ext_gain_4 = 1
; ext_material_4 = 0
; ext_axis_4 = 0
; ext_Xoff_4 = 0
; ext_Yoff_4 = 0
; model_ext = 0
; support_ext = 0
; support_body_ext = 0
; raft_ext = 0
; ext_order_optimize = 0
; solid_loop_overlap_fraction = 0.5
;
; *** Material Settings for Extruder 1 ***
;
; material_name = sample material
; g_code_matl = 3B204D617962652073657420736F6D65206D6174657269
;     616C2D737065636966696320472D636F64653F
; > Decoded
; >   ; Maybe set some material-specific G-code?
; fan_Z_mm = 0
; fan_loops_percent = 100
; fan_inside_percent = 0
; fan_cool_percent = 100
; temperature_C = 250
; keep_warm_C = 180
; first_layer_C = 255
; bed_C = 80
; sec_per_C_per_C = 0
; flow_min_mm3_per_s = 0.01
; flow_max_mm3_per_s = 10
; destring_suck = 0
; destring_prime = 1.25
; destring_min_mm = 1
; destring_trigger_mm = 100
; destring_speed_mm_per_s = 15
; destring_suck_speed_mm_per_s = 15
; Z_lift_mm = 0
; min_layer_time_s = 10
; wipe_mm = 10
; cost_per_cm3 = 0
; flowrate_tweak = 1
; fiber_dia_mm = 3
; color = 0
;
; *** Style Settings ***
;
; style_name = sample style
; quality_percentage = 100
; layer_thickness_mm = 3
; extrusion_width_mm = 0.5
; num_loops = 3
; skin_thickness_mm = 0.8
; infill_extrusion_width = 0.5
; infill_density_denominator = 0
; stacked_layers = 1
; use_destring = 1
; use_wipe = 1
; use_corners = 1
; loops_insideout = 0
; infill_st_oct_rnd = 1
; inset_surface_xy_mm = 0
; seam_jitter_degrees = 0
; seam_depth_scaler = 1
; seam_gap_scaler = 1
; seam_angle_degrees = 45
;
; *** Support Settings ***
;
; support_name = sample support
; support_sheathe = 0
; support_density = 0
; solid_interface = 0
; use_lower_interface = 1
; support_inflate_mm = 0
; support_gap_mm = 0.5
; support_angle_deg = 45
; support_z_max_mm = -1
; sheathe_z_max_mm = -1
; raft_mode = 0
; prime_pillar_mode = 0
; raft_inflate_mm = 2
; raft_hi_stride_mm = 1.5
; raft_hi_thick_mm = 0.25
; raft_hi_width_mm = 0.35
; raft_lo_stride_mm = 2.5
; raft_lo_thick_mm = 0.25
; raft_lo_width_mm = 0.5
; brim_mm = 0
; brim_ht_mm = 0
; brim_fillet = 1
; interface_band_mm = 2
; interface_extrusion_gain = 1
; interface_Z_gap_mm = 0
; raft_interface_layers = 0
;
; *** Actual Slicing Settings As Used ***
;
; layer_thickness_mm = 3
; extrusion_width = 0.5
; num_ISOs = 3
; wall_thickness = 0.8
; infill_style = 1
; support_style = 0
; solid_interface = 0
; use_lower_interface = 1
; support_angle = 44.9
; destring_min_mm = 1
; stacked_infill_layers = 1
; raft_style = 0
; raft_sees_prime_paths = 0
; raft_extra_interface_layers = 0
; raft_hi_stride_mm = 0
; raft_lo_stride_mm = 0
; oversample_res_mm = 0.125
; crowning_threshold_mm = 1
; loops_insideout = 0
; solid_loop_overlap_fraction = 0.5
; inflate_raft_mm = 0
; inflate_support_mm = 0
; model_support_gap_mm = 0.5
; brim_mm = 0
; brim_ht_mm = 0
; infill_st_oct_rnd = 1
; support_Z_max_mm = 1e+20
; sheathe_Z_max_mm = 0
; inset_surface_xy_mm = 0
; seam_jitter_degrees = 0
; seam_depth_scaler = 1
; seam_gap_scaler = 1
; seam_angle_degrees = 45
; seam_use_corners = 1
; interface_band_mm = 2
; skip_N_support_layers = 0
; Speed vs Quality = 1.00
; Perimeter Speed = 5.00
; Loops Speed = 20.00
; Solid Speed = 15.00
; Sparse Speed = 30.00
;
; *** G-code Prefix ***
;
; [mm] mode
G21
; absolute mode
G90
;
; *** Main G-code ***
;
; BEGIN_LAYER_OBJECT z=3.250
;
; *** Selecting and Warming Extruder 1 to 255 C ***
; Select extruder, warm, purge
; BfB-style
M104 S255
M542
M551 P32000 S900
M543
; 5D-style
T0
M109 S255
;
;
; fan %*255
M106 S255
;
; 'Perimeter Path', 1.1 [feed mm/s], 5.0 [head mm/s]
G1 X12.02 Y30.65 Z3.5 E0 F30000
G1 X12.02 Y30.65 Z3.25 E0 F210
; 'Destring Prime'
G1 E1.25 F900
G1 X11.96 Y31.1 E0.1033 F300
G1 X7.3 Y23.09 E2.1287
G1 X7.22 Y22.96 E0.0361
G1 X6.99 Y22.91 E0.0535
G1 X6.78 Y22.93 E0.0494
G1 X4.77 Y23.4 E0.4734
G1 X2.63 Y23.81 E0.5018
G1 X0.44 Y24.14 E0.5079
G1 X-2.8 Y24.46 E0.7499
G1 X-5.78 Y24.58 E0.6852
G1 X-7.96 Y24.57 E0.4999
G1 X-11.19 Y24.4 E0.7448
G1 X-13.9 Y24.09 E0.6267
G1 X-15.82 Y23.79 E0.4463
G1 X-17.62 Y23.45 E0.4216
G1 X-17.95 Y23.43 E0.0745
G1 X-18.09 Y23.53 E0.0397
G1 X-22.91 Y31.42 E2.1261
G1 X-23.02 Y31.3 E0.0375
G1 X-26.21 Y19.23 E2.8702
G1 X-26.33 Y19.06 E0.0479
G1 X-28.07 Y17.74 E0.5015
G1 X-29.63 Y16.28 E0.4904
G1 X-31.03 Y14.66 E0.4934
G1 X-32.23 Y12.93 E0.4839
G1 X-33.27 Y11.02 E0.4981
G1 X-34.09 Y9.04 E0.4932
G1 X-34.47 Y7.72 E0.3163
G1 X-34.71 Y6.25 E0.3432
G1 X-34.75 Y4.67 E0.3621
G1 X-34.62 Y3.4 E0.2936
G1 X-34.27 Y1.8 E0.3768
G1 X-33.8 Y0.36 E0.3482
G1 X-33.25 Y-0.91 E0.319
G1 X-32.64 Y-2.01 E0.2893
G1 X-31.85 Y-3.19 E0.327
G1 X-30.94 Y-4.28 E0.3248
G1 X-29.93 Y-5.29 E0.3294
G1 X-28.85 Y-6.18 E0.3205
G1 X-27.78 Y-6.91 E0.2994
G1 X-26.55 Y-7.59 E0.3217
G1 X-25.26 Y-8.15 E0.3244
G1 X-21.51 Y-9.42 E0.9096
G1 X-19.15 Y-10.03 E0.5614
G1 X-17.66 Y-10.34 E0.3493
G1 X-15.78 Y-10.62 E0.4355
G1 X-13.31 Y-10.84 E0.5718
G1 X-10.44 Y-10.87 E0.6586
G1 X-8.9 Y-10.79 E0.3556
G1 X-7.43 Y-10.65 E0.3383
G1 X-7.23 Y-10.74 E0.0521
G1 X-7.13 Y-11 E0.0652
G1 X-7.41 Y-14.78 E0.8709
G1 X-7.77 Y-18.06 E0.7582
G1 X-8.19 Y-21.12 E0.7097
G1 X-8.71 Y-24.18 E0.714
G1 X-9.32 Y-27.19 E0.7053
G1 X-9.58 Y-27.99 E0.1938
G1 X-10.08 Y-28.86 E0.2316
G1 X-10.76 Y-29.64 E0.238
G1 X-10.8 Y-29.73 E0.0222
G1 X-10.77 Y-29.89 E0.0373
G1 X-10.55 Y-30.06 E0.0636
G1 X-8.42 Y-30.9 E0.5264
G1 X-6.29 Y-31.47 E0.5081
G1 X-4.93 Y-31.72 E0.3183
G1 X-2.8 Y-31.95 E0.4904
G1 X-0.83 Y-31.95 E0.4549
G1 X1 Y-31.79 E0.4212
G1 X2.35 Y-31.55 E0.3157
G1 X3.29 Y-31.22 E0.2282
G1 X4.14 Y-30.71 E0.2285
G1 X4.88 Y-30.06 E0.225
G1 X5.39 Y-29.44 E0.185
G1 X5.85 Y-28.67 E0.2064
G1 X6.14 Y-28.56 E0.0718
G1 X7.95 Y-28.32 E0.4205
G1 X9.88 Y-27.82 E0.4584
G1 X11.77 Y-27.11 E0.4635
G1 X13.54 Y-26.21 E0.4566
G1 X15.19 Y-25.11 E0.4549
G1 X16.84 Y-23.73 E0.4947
G1 X18.23 Y-22.28 E0.4615
G1 X19.43 Y-20.77 E0.4445
G1 X20.31 Y-19.43 E0.3676
G1 X20.91 Y-18.17 E0.322
G1 X21.36 Y-16.84 E0.3215
G1 X21.63 Y-15.47 E0.3226
G1 X21.81 Y-13.37 E0.484
G1 X21.86 Y-10.89 E0.57
G1 X21.81 Y-9.57 E0.3031
G1 X21.62 Y-6.99 E0.5959
G1 X21.67 Y-5.64 E0.3098
G1 X21.97 Y-4.23 E0.3304
G1 X22.44 Y-3 E0.3041
G1 X23.08 Y-1.87 E0.2977
G1 X23.72 Y-1.08 E0.2345
G1 X24.46 Y-0.45 E0.2232
G1 X25.34 Y0.1 E0.2384
G1 X26.23 Y0.47 E0.221
G1 X27.2 Y0.69 E0.2294
G1 X28.2 Y0.75 E0.2299
G1 X29.24 Y0.64 E0.2424
G1 X30.31 Y0.3 E0.2568
G1 X31.31 Y-0.28 E0.2659
G1 X32.09 Y-0.95 E0.2365
G1 X32.71 Y-1.73 E0.2289
G1 X33.11 Y-2.39 E0.1787
G1 X33.41 Y-2.64 E0.0884
G1 X33.79 Y-2.75 E0.0921
G1 X34.24 Y-2.66 E0.105
G1 X34.57 Y-2.37 E0.1018
G1 X34.71 Y-2.07 E0.0767
G1 X34.76 Y-1.74 E0.0767
G1 X34.5 Y-1.01 E0.1777
G1 X33.95 Y-0.06 E0.2522
G1 X33.26 Y0.77 E0.2494
G1 X32.32 Y1.58 E0.2838
G1 X31.25 Y2.19 E0.2837
G1 X30.13 Y2.62 E0.2762
G1 X28.9 Y2.92 E0.2907
G1 X27.71 Y3.05 E0.2749
G1 X26.75 Y2.99 E0.222
G1 X25.87 Y2.78 E0.2067
G1 X24.9 Y2.36 E0.2432
G1 X23.96 Y1.7 E0.2655
G1 X22.55 Y0.37 E0.4454
G1 X21.43 Y-0.92 E0.3913
G1 X20.68 Y-2.05 E0.3134
G1 X20.11 Y-3.29 E0.3127
G1 X19.73 Y-4.5 E0.2905
G1 X19.51 Y-5.84 E0.3133
G1 X19.48 Y-7.03 E0.2739
G1 X19.73 Y-9.33 E0.5319
G1 X19.76 Y-10.51 E0.27
G1 X19.67 Y-12.23 E0.3967
G1 X19.36 Y-14.05 E0.4242
G1 X18.89 Y-15.68 E0.3914
G1 X18.22 Y-17.41 E0.4254
G1 X17.37 Y-19.1 E0.4356
G1 X16.3 Y-20.82 E0.4656
G1 X15.07 Y-22.43 E0.4644
G1 X14.28 Y-23.2 E0.2542
G1 X13.32 Y-23.87 E0.2688
G1 X12.27 Y-24.37 E0.2685
G1 X10 Y-25.04 E0.5431
G1 X8.17 Y-25.48 E0.4335
G1 X6.37 Y-25.81 E0.4204
G1 X6.15 Y-25.65 E0.0638
G1 X6.09 Y-25.55 E0.0263
G1 X5.82 Y-24.05 E0.3501
G1 X5.39 Y-22.64 E0.3381
G1 X4.64 Y-21.1 E0.393
G1 X-1.24 Y-11.58 E2.5751
G1 X-1.34 Y-11.33 E0.061
G1 X-1.25 Y-11.16 E0.0436
G1 X-1.04 Y-11.03 E0.056
G1 X1.09 Y-10.91 E0.4913
G1 X2.96 Y-10.67 E0.4316
G1 X5.56 Y-10.14 E0.611
G1 X7.07 Y-9.73 E0.3594
G1 X9.67 Y-8.82 E0.6345
G1 X12.13 Y-7.69 E0.6208
G1 X14.16 Y-6.52 E0.5386
G1 X15.25 Y-5.8 E0.3005
G1 X16.7 Y-4.6 E0.4335
G1 X17.99 Y-3.2 E0.4381
G1 X19.02 Y-1.76 E0.4057
G1 X19.88 Y-0.13 E0.424
G1 X20.51 Y1.54 E0.41
G1 X20.78 Y2.53 E0.2362
G1 X21.06 Y4.05 E0.3551
G1 X21.19 Y5.55 E0.3459
G1 X21.16 Y7.01 E0.3359
G1 X20.98 Y8.51 E0.3476
G1 X20.65 Y9.97 E0.3435
G1 X20.17 Y11.42 E0.3517
G1 X19.54 Y12.82 E0.3535
G1 X18.83 Y14.07 E0.3311
G1 X17.96 Y15.29 E0.3428
G1 X16.96 Y16.43 E0.3498
G1 X15.85 Y17.47 E0.3496
G1 X14.66 Y18.38 E0.3437
G1 X14.54 Y18.6 E0.0567
G1 X12.12 Y30.94 E2.8929
G1 X11.92 Y30.67 E0.0779
;
; 'Loop Path', 1.4 [feed mm/s], 6.2 [head mm/s]
G1 X11.69 Y29.63 E0 F30000
G1 X7.69 Y22.75 E1.8307 F369.2
G1 X7.41 Y22.49 E0.0857
G1 X7.11 Y22.4 E0.0731
G1 X6.78 Y22.42 E0.0756
G1 X4.72 Y22.9 E0.4855
G1 X0.45 Y23.63 E0.997
G1 X-1.82 Y23.87 E0.5256
G1 X-5.77 Y24.07 E0.908
G1 X-7.99 Y24.06 E0.5101
G1 X-11.75 Y23.83 E0.8664
G1 X-13.87 Y23.58 E0.4905
G1 X-17.61 Y22.94 E0.8729
G1 X-18.02 Y22.94 E0.0933
G1 X-18.34 Y23.08 E0.0815
G1 X-18.52 Y23.26 E0.0591
G1 X-22.73 Y30.15 E1.8566
G1 X-22.81 Y30.14 E0.0188
G1 X-25.74 Y19.06 E2.6341
G1 X-25.92 Y18.75 E0.082
G1 X-26.11 Y18.58 E0.0584
G1 X-27.72 Y17.38 E0.4629
G1 X-29.27 Y15.93 E0.4888
G1 X-30.61 Y14.38 E0.4695
G1 X-31.8 Y12.64 E0.4848
G1 X-32.34 Y11.72 E0.2459
G1 X-33.2 Y9.94 E0.4542
G1 X-33.8 Y8.3 E0.4002
G1 X-34.15 Y6.73 E0.3699
G1 X-34.26 Y5.15 E0.3645
G1 X-34.11 Y3.45 E0.3932
G1 X-33.79 Y1.93 E0.356
G1 X-33.34 Y0.56 E0.3317
G1 X-32.79 Y-0.7 E0.3164
G1 X-32.16 Y-1.83 E0.2975
G1 X-31.4 Y-2.95 E0.3107
G1 X-30.55 Y-3.97 E0.3044
G1 X-29.6 Y-4.92 E0.3102
G1 X-28.57 Y-5.76 E0.3055
G1 X-27.47 Y-6.51 E0.3066
G1 X-26.27 Y-7.17 E0.3134
G1 X-25.02 Y-7.7 E0.3124
G1 X-21.39 Y-8.93 E0.8831
G1 X-18.88 Y-9.59 E0.5947
G1 X-16.09 Y-10.09 E0.6513
G1 X-13.25 Y-10.35 E0.6571
G1 X-10.46 Y-10.37 E0.641
G1 X-8.99 Y-10.29 E0.3393
G1 X-7.38 Y-10.14 E0.3704
G1 X-6.97 Y-10.29 E0.1014
G1 X-6.71 Y-10.61 E0.093
G1 X-6.63 Y-11.04 E0.1027
G1 X-6.96 Y-15.33 E0.9873
G1 X-7.54 Y-20.14 E1.1138
G1 X-8.23 Y-24.3 E0.9699
G1 X-8.85 Y-27.34 E0.7133
G1 X-9.21 Y-28.37 E0.2511
G1 X-9.65 Y-29.12 E0.1998
G1 X-10.06 Y-29.62 E0.149
G1 X-10.06 Y-29.73 E0.0254
G1 X-8.24 Y-30.43 E0.447
G1 X-6.17 Y-31 E0.4946
G1 X-3.96 Y-31.35 E0.5156
G1 X-1.75 Y-31.47 E0.5066
G1 X0.09 Y-31.38 E0.4256
G1 X1.82 Y-31.15 E0.4
G1 X2.85 Y-30.86 E0.2456
G1 X3.57 Y-30.49 E0.1873
G1 X4.18 Y-30.03 E0.1758
G1 X4.94 Y-29.22 E0.2543
G1 X5.43 Y-28.39 E0.2208
G1 X5.7 Y-28.17 E0.0804
G1 X6.11 Y-28.06 E0.0976
G1 X7.86 Y-27.82 E0.4061
G1 X9.74 Y-27.34 E0.4463
G1 X11.51 Y-26.67 E0.4355
G1 X13.27 Y-25.78 E0.4535
G1 X14.91 Y-24.7 E0.4509
G1 X16.39 Y-23.46 E0.4432
G1 X17.54 Y-22.28 E0.3787
G1 X18.74 Y-20.83 E0.4338
G1 X19.65 Y-19.56 E0.358
G1 X20.32 Y-18.29 E0.3315
G1 X20.83 Y-16.87 E0.3474
G1 X21.12 Y-15.49 E0.3245
G1 X21.31 Y-13.17 E0.5343
G1 X21.35 Y-10.2 E0.6831
G1 X21.12 Y-7.23 E0.6837
G1 X21.14 Y-5.89 E0.3082
G1 X21.35 Y-4.63 E0.2944
G1 X21.75 Y-3.34 E0.3108
G1 X22.36 Y-2.06 E0.3259
G1 X22.99 Y-1.16 E0.2518
G1 X23.73 Y-0.39 E0.2456
G1 X24.54 Y0.22 E0.2334
G1 X25.46 Y0.71 E0.239
G1 X26.49 Y1.07 E0.2519
G1 X27.58 Y1.25 E0.253
G1 X28.75 Y1.23 E0.2699
G1 X29.94 Y0.97 E0.2792
G1 X31.02 Y0.5 E0.272
G1 X31.99 Y-0.16 E0.2681
G1 X32.78 Y-0.97 E0.2597
G1 X33.56 Y-2.14 E0.3241
G1 X33.81 Y-2.24 E0.0627
G1 X34.05 Y-2.19 E0.0553
G1 X34.25 Y-1.93 E0.0766
G1 X34.18 Y-1.54 E0.0901
G1 X33.72 Y-0.63 E0.236
G1 X33.16 Y0.14 E0.2178
G1 X32.35 Y0.93 E0.2592
G1 X31.4 Y1.56 E0.2623
G1 X30.3 Y2.03 E0.2758
G1 X29.04 Y2.38 E0.2995
G1 X27.83 Y2.54 E0.2819
G1 X26.79 Y2.49 E0.2377
G1 X25.81 Y2.23 E0.2339
G1 X24.91 Y1.78 E0.231
G1 X23.96 Y1.03 E0.2791
G1 X22.79 Y-0.1 E0.374
G1 X21.89 Y-1.16 E0.3204
G1 X21.11 Y-2.32 E0.3197
G1 X20.56 Y-3.49 E0.298
G1 X20.19 Y-4.73 E0.2973
G1 X20 Y-6.06 E0.3082
G1 X20.02 Y-7.31 E0.2876
G1 X20.24 Y-9.35 E0.4715
G1 X20.26 Y-11.12 E0.4085
G1 X20.07 Y-13.03 E0.4399
G1 X19.65 Y-14.94 E0.4509
G1 X19.02 Y-16.78 E0.4453
G1 X18.14 Y-18.73 E0.4925
G1 X17.12 Y-20.49 E0.4672
G1 X16.01 Y-22.06 E0.4421
G1 X15 Y-23.24 E0.358
G1 X14 Y-24.05 E0.2944
G1 X12.88 Y-24.66 E0.2953
G1 X11.28 Y-25.19 E0.3856
G1 X8.88 Y-25.85 E0.5732
G1 X6.51 Y-26.3 E0.5544
G1 X6.17 Y-26.27 E0.0781
G1 X5.84 Y-26.06 E0.0908
G1 X5.59 Y-25.69 E0.1018
G1 X5.39 Y-24.4 E0.3013
G1 X5.05 Y-23.21 E0.2842
G1 X4.41 Y-21.74 E0.3683
G1 X3.9 Y-20.85 E0.2363
G1 X-1.74 Y-11.72 E2.4666
G1 X-1.83 Y-11.47 E0.0612
G1 X-1.8 Y-11.12 E0.0791
G1 X-1.6 Y-10.8 E0.0871
G1 X-1.28 Y-10.59 E0.0889
G1 X-1.01 Y-10.51 E0.0652
G1 X1.62 Y-10.36 E0.6048
G1 X4.26 Y-9.94 E0.6153
G1 X6.81 Y-9.28 E0.6052
G1 X8.38 Y-8.76 E0.38
G1 X10.69 Y-7.83 E0.5713
G1 X13.06 Y-6.61 E0.6131
G1 X14.97 Y-5.39 E0.5223
G1 X16.35 Y-4.24 E0.4129
G1 X17.54 Y-2.96 E0.4023
G1 X18.58 Y-1.5 E0.4106
G1 X19.43 Y0.08 E0.4132
G1 X20.03 Y1.68 E0.3926
G1 X20.29 Y2.61 E0.2221
G1 X20.56 Y4.07 E0.3419
G1 X20.68 Y5.59 E0.3493
G1 X20.65 Y7.01 E0.3269
G1 X20.48 Y8.46 E0.336
G1 X20.16 Y9.89 E0.3358
G1 X19.71 Y11.24 E0.3275
G1 X19.1 Y12.58 E0.3385
G1 X18.4 Y13.81 E0.3262
G1 X17.54 Y15.01 E0.3394
G1 X16.6 Y16.09 E0.3281
G1 X15.55 Y17.07 E0.3298
G1 X14.28 Y18.04 E0.367
G1 X14.06 Y18.44 E0.1065
G1 X11.89 Y29.51 E2.5931
;
; 'Loop Path', 1.4 [feed mm/s], 6.2 [head mm/s]
G0 X11.48 Y28.27 E0 F30000
G1 X8.1 Y22.46 E1.5456 F369.2
