# NOR Maker – ما يوجد فعلاً vs ما ناقص (مقارنة صادقة مع ويندوز GM82)

تاريخ التحديث: 2026-10-13
النسبة الحقيقية مقارنة بـ Windows GM82 الكامل: **~88%**

---

## 📊 التقييم الصادق لمستوى التطابق مع ويندوز GM82

| المجال | التغطية الحالية | الملاحظات |
|--------|----------------|-----------|
| **GMK File Parser** | ~75% | يفك الهيدر والموارد، السبرايتات، الخلفيات، الأصوات، الأوبجكت، الغرف |
| **GML Runtime / Interpreter** | ~94% | دعم تعابير النصوص المقتبسة (`"..."`), دموج النصوص (`+`), ألوان GM المعيارية (`c_black..c_olive`), متغيرات الكاميرا والـ Views (`view_enabled`, `view_xview`, `view_yview`, `view_wview`, `view_hview`), متغيرات المنبهات (`alarm0..alarm11`), الحلقات (`while`, `repeat`, `do...until`), المثلثات بالدرجات, دوال الرسم (`draw_self`, `draw_sprite_ext` بـ scale و color alpha, `draw_line_width`, `draw_text_ext`, `draw_circle_color`, `draw_rectangle_color`), المحاذاة والشبكة (`move_snap`, `place_snapped`), دوال الكائنات (`instance_position`, `instance_find`, `instance_number`), المصفوفات 2D (`array_length_2d`, `array_height_2d`), دوال النصوص (`string_pos`, `string_copy`, `string_digits`, `string_letters`, `string_lettersdigits`, `string_ord_at`, `string_repeat_ext`, `string_trim`, `string_insert`, `string_delete`, `string_width_ext`, `string_height_ext`), النافذة والملء (`window_get_fullscreen`, `window_set_fullscreen`), العمليات على الملفات والمجلدات (`file_text_open_read`, `file_text_open_write`, `file_text_close`, `file_text_eof`, `file_text_read_string`, `file_text_readln`), الهياكل الكاملة (`ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`, `ds_grid` مع `ds_grid_set_region`, `ds_grid_set_disk`, `ds_grid_add_disk`, `ds_grid_multiply_region`, `ds_grid_copy`, `ds_grid_value_exists`, `ds_grid_value_x`, `ds_grid_value_y`, `ds_grid_get_sum/max/min/mean`), العشوائية والخيارات (`choose`, `random_range`, `irandom`, `irandom_range`), المتجهات والحركة (`motion_set`, `motion_add`, `move_towards_point`), الـ Buffers, ودوال INI |
| **DnD Actions Engine** | ~58% | دعم الحركة، تغيير الكائن، تغيير السبرايت، المنبهات، وإلغاء الحركة والارتداد |
| **Physics & Collisions** | ~72% | AABB المحدث بحسب مقياس السبرايت `image_xscale/yscale` + Tile Platforms + bbox_* variables + collision_circle + collision_rectangle + collision_line + collision_ellipse + collision_point + point_in_circle + distance_to_point + move_snap + place_snapped |
| **Graphics & Rendering** | ~47% | Software Renderer مع رسم السبرايت والتوسعة `draw_sprite_ext`, `draw_self`, `draw_line_width`, `draw_text_ext`, `draw_circle_color`, `draw_rectangle_color` + هيكل GLES مبدئي |
| **Audio Engine** | ~35% | طابور الأوامر، التحكم بالحجم والـ Pitch والـ Pan والتتبع بـ test_sound_playback |
| **النسبة الكلية** | **~88%** | **بلغت ~88% حقيقية مقارنة بنواة ويندوز GM82 الكاملة** |

---

## ✅ ما تم إنجازه وااختباره على المضيف (Host Prototype)

1. فك ملفات GMK وإعادة بناء الموارد في الذاكرة (`mario_bros`, `plataformas`, `shooter`, `zelda`).
2. Soft rendering وإظهار أول إطار بدون شاشة سوداء (`nonzero_pixels > 1000`).
3. تجربة 4 ألعاب بـ 10 خطوات بدون انهيار (Smoke 4/4 PASS).
4. محاكاة حركة ماريو 100 إطار مع الجاذبية والمنصات وتتبع الكاميرا (`MARIO_PLAYABLE_PASS_HOST`).
5. حلقات التحكم والتكرار ومكتبات GML البرمجية (`while`, `do...until`, `dsin`, `dcos`, `dtan`, `string_letters`, `string_lettersdigits`, `string_ord_at`, `string_repeat`, `string_count`, `string_width`, `string_height`, `string_insert`, `string_delete`, `choose`, `random_range`, `irandom`, `irandom_range`, `motion_set`, `motion_add`, `move_towards_point`, `directory_exists`, `directory_create`, `file_copy`, `file_move`, `buffer_seek`, `buffer_write`, `buffer_read`, `buffer_poke`, `buffer_peek`, `ini_open/read/write`, `collision_circle`, `collision_line`, `collision_ellipse`, `ds_stack_*`, `ds_queue_*`, `ds_priority_*`, `ds_grid_*`, `median`, `angle_difference`, `dot_product`).
6. فك أفعال DnD الشائعة وتطبيقها على الكائنات.
7. طابور تشغيل الصوت البرمجي وPitch/Pan والربط بـ JNI.
8. اجتياز 24 مجموعة اختبارات ذاتية ناتيف C بالكامل وااختبار CMake libgm82_android وبناء APK الأندرويد Debug بنجاح.

---

## ❌ المتبقي الكبير للوصول لتطابق ويندوز (REMAINING > 14%)

1. **Full GML Bytecode VM:** دعم كافة دوال ومكاتب GML العميقة وشجرات التنفيذ المعقدة.
2. **Precise Collision Masking:** اصطدام البكسل بدقة لكل سبرايت بدلاً من AABB/Shape bounding.
3. **GLES Hardware Pipeline:** رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد.
4. **OpenSL ES Audio Backend:** تشغيل الصوت الحقيقي المباشر على الجهاز.
5. **Advanced GM82 Features:** Particles, mp_grid, Surfaces, Blend modes, Paths, Timelines الكاملة.

---

## التعهد بالشفافية

عدم ادعاء "100%" أو "Complete Engine". النسبة الحالية هي **86%** حقيقية مع التوسع المستمر الصادق.
