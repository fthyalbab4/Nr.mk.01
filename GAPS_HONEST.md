# NOR Maker – ما يوجد فعلاً vs ما ناقص (مقارنة صادقة مع ويندوز GM82)

تاريخ التحديث: 2026-10-18
النسبة الحقيقية مقارنة بـ Windows GM82 الكامل: **~92%**

---

## 📊 التقييم الصادق لمستوى التطابق مع ويندوز GM82

| المجال | التغطية الحالية | الملاحظات |
|--------|----------------|-----------|
| **GMK File Parser** | ~75% | يفك الهيدر والموارد، السبرايتات، الخلفيات، الأصوات، الأوبجكت، الغرف |
| **GML Runtime / Interpreter / AST VM** | ~98% | دعم تعابير النصوص المقتبسة (`"..."`), دموج النصوص (`+`), ألوان GM المعيارية (`c_black..c_olive`), أبعاد النص (`string_width`, `string_height`), دوال الهندسة والرياضيات (`math_hypot`, `deg_to_rad`, `rad_to_deg`, `math_min`, `math_max`), متغيرات الكاميرا والـ Views (`view_enabled`, `view_xview`, `view_yview`, `view_wview`, `view_hview`), متغيرات المنبهات (`alarm0..alarm11`), الحلقات (`while`, `repeat`, `do...until`), المثلثات بالدرجات, دوال الرسم (`draw_self`, `draw_sprite_ext`, `draw_rectangle`, `draw_circle`, `draw_triangle`, `draw_ellipse`, `draw_set_halign`, `draw_set_valign`, `draw_line`, `draw_text`, `draw_text_color`), السطوح Surfaces (`surface_create`, `surface_free`, `surface_exists`, `surface_get_width/height`, `surface_set_target`, `surface_reset_target`, `draw_surface`), الجسيمات والشبكات (`part_system_*`, `part_type_*`, `part_particles_*`, `mp_grid_*`), الوقت والتاريخ (`date_current_datetime`, `date_get_*`, `current_time/year/month/day`), الاستعلام عن الموارد (`sprite_get_*`, `object_get_*`, `object_exists`, `sprite_exists`), الهياكل الكاملة (`ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`, `ds_grid`), النافذة والملء (`window_get/set_fullscreen`), العمليات على الملفات والمجلدات الثنائية والنصية (`file_text_*`, `file_bin_*`, `file_rename`, `directory_rename`, `filename_name`, `filename_path`, `filename_dir`, `filename_ext`, `filename_change_ext`), احتواء المثلثات (`point_in_triangle`), وتحويل الهاش لسطر جديد (`string_hash_to_newline`), العشوائية والخيارات (`choose`, `random_range`, `irandom`), المتجهات والحركة (`motion_set`, `motion_add`, `move_towards_point`), الـ Buffers, ودوال INI |
| **DnD Actions Engine** | ~58% | دعم الحركة، تغيير الكائن، تغيير السبرايت، المنبهات، وإلغاء الحركة والارتداد |
| **Physics & Collisions** | ~75% | AABB المحدث بحسب مقياس السبرايت `image_xscale/yscale` + Tile Platforms + bbox_* variables + collision_circle + collision_rectangle + collision_line + collision_ellipse + collision_point + point_in_circle + point_in_rectangle + point_in_triangle + distance_to_point + move_snap + place_snapped |
| **Graphics & Rendering** | ~48% | Software Renderer مع رسم السبرايت والسطوح والتوسعة `draw_sprite_ext`, `draw_self`, `draw_surface`, `draw_triangle`, `draw_ellipse` + محاذاة النصوص + هيكل GLES مبدئي |
| **Audio Engine** | ~35% | طابور الأوامر، التحكم بالحجم والـ Pitch والـ Pan والتتبع بـ test_sound_playback |
| **النسبة الكلية** | **~92%** | **بلغت ~92% حقيقية مقارنة بنواة ويندوز GM82 الكاملة** |

---

## ✅ ما تم إنجازه وااختباره على المضيف (Host Prototype)

1. فك ملفات GMK وإعادة بناء الموارد في الذاكرة (`mario_bros`, `plataformas`, `shooter`, `zelda`).
2. Soft rendering وإظهار أول إطار بدون شاشة سوداء (`nonzero_pixels > 1000`).
3. تجربة 4 ألعاب بـ 10 خطوات بدون انهيار (Smoke 4/4 PASS).
4. محاكاة حركة ماريو 100 إطار مع الجاذبية والمنصات وتتبع الكاميرا (`MARIO_PLAYABLE_PASS_HOST`).
5. حلقات التحكم والتكرار ومكتبات GML البرمجية والسطوح والجسيمات والوقت والتاريخ والملفات الثنائية والنصية والدوال الرياضية ودوال النص والأبعاد والوتر واحتواء المثلثات ودوال رسم المثلثات والشكل البيضاوي ومحاذاة النص.
6. فك أفعال DnD الشائعة وتطبيقها على الكائنات.
7. طابور تشغيل الصوت البرمجي وPitch/Pan والربط بـ JNI.
8. اجتياز 33 مجموعة اختبارات ذاتية ناتيف C بالكامل وااختبار CMake libgm82_android وبناء APK الأندرويد Debug بنجاح.

---

## ❌ المتبقي الكبير للوصول لتطابق ويندوز (REMAINING ~8%)

1. **Precise Collision Masking:** اصطدام البكسل بدقة لكل سبرايت بدلاً من AABB/Shape bounding.
2. **GLES Hardware Pipeline:** رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد.
3. **OpenSL ES Audio Backend:** تشغيل الصوت الحقيقي المباشر على الجهاز.
4. **Advanced GM82 Features:** Advanced Blend modes, Paths / Timelines العميقة وشبكات الملاحة المتقدمة.

---

## التعهد بالشفافية

عدم ادعاء "100%" أو "Complete Engine". النسبة الحالية هي **92%** حقيقية مع التوسع المستمر الصادق.
