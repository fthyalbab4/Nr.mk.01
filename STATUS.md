# تقدم مشروع NOR Maker / GM82

تاريخ التحديث: 2026-10-24
النسبة الفعلية مقارنة بـ Windows GM82 الكامل: **~94.8%**

## 📌 ملخص الوضع الحالي
- النواة تتطور كنسخة أولية على Host و Android (تفك GMK، تمارس Soft Render، تحاكي فيزياء ماريو، تنفذ أفعال DnD وحلقات GML التحكمية `while`, `do...until`, والتعابير النصية المقتبسة `"..."` والربط النصي `+` وثوابت الألوان المعيارية `c_black..c_olive` ومتغيرات الكاميرا `view_xview/yview/wview/hview/enabled` والمنبهات `alarm0..11`, ودوال المثلثات بالدرجات `dsin`, `dcos`, `dtan`, ودوال النصوص `string_pos`, `string_copy`, `string_digits`, `string_letters`, `string_lettersdigits`, `string_ord_at`, `string_byte_at`, `string_byte_length`, `string_replace`, `string_replace_all`, `string_repeat_ext`, وإدارة الملفات والمجلدات `file_exists`, `file_delete`, `file_copy`, `file_move`, `file_rename`, `directory_exists`, `directory_create`, `directory_rename`, `file_text_open_read`, `file_text_open_write`, `file_text_open_append`, `file_text_close`, `file_text_eof`, `file_text_write_string`, `file_text_write_real`, `file_text_writeln`, `file_text_read_string`, `file_text_read_real`, `file_text_readln`, وحساب المسافات والزوايا `angle_difference`, `dot_product`, `hypot`, `point_in_rectangle`, ومتغيرات الفيزياء وحزم الهياكل البياناتية `ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`, `ds_grid` مع `ds_grid_value_disk_*`, ودوال إنشاء وإنهاء الكائنات `instance_create`, `instance_destroy`, `instance_exists`, `instance_nearest`, `instance_place`, وتشغيل ومتابعة الصوت `sound_play`, `sound_stop`, `sound_isplaying`, `sound_volume`, `sound_pan`, `sound_pitch`, والتنقل بين الغرف `room_goto`, `room_goto_next`, `room_restart`, `game_end`, `game_restart`, ومتغيرات الماوس واللعبة `mouse_x`, `mouse_y`, `score`, `lives`, `health`, `room_speed`, `room_width`, `room_height`, والـ Surfaces والجسيمات وشبكات الملاحة والوقت والتاريخ والبافرات والملفات الثنائية والنصية).
- اجتياز 38 اختبار ذاتي ناتيف بالكامل وبناء المكتبة الناتيف `libgm82_android.so` عبر CMake وتجميع تطبيق الأندرويد `app-debug.apk` عبر Gradle بنجاح 100%.
- النسبة الإجمالية مقارنة بالمحرك الكامل لويندوز هي **94.8%**.

## 🗺️ خطة التطوير الشاملة للوصول لـ Core & GML Full Support

### المرحلة 1: مفسر وVM الـ GML (GML Bytecode VM Engine)
- دعم كامل لكافة تعابير ودوال GML العميقة وتمرير المعاملات المتقدمة ودوال المثلثات والنصوص بالدرجات ورمز المحارف `string_ord_at`, `string_byte_at`, `string_byte_length`.
- تحسين التعامل مع المصفوفات ثنائية الأبعاد والبُنى البياناتية المتعددة (`ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`, `ds_grid`) والـ Buffers (`buffer_create`, `buffer_write`, `buffer_read`, `buffer_poke`, `buffer_peek`, `buffer_get_size`, `buffer_seek`).

### المرحلة 2: الاصطدامات الدقيقة (Precise Collision Masking)
- دعم الأشكال الصدامية `collision_line` و`collision_ellipse` و`collision_circle` و`collision_rectangle` و`collision_point` و`distance_to_point`.
- الانتقال المستقبلي إلى Per-Pixel Masking لكل سبرايت.

### المرحلة 3: معالجة العرض والجرافيكس العتادي (GLES Hardware Pipeline)
- رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد عبر OpenGL ES.
- دعم الـ Surfaces والـ Blend Modes وشاشات الـ CRT Shaders.

### المرحلة 4: تشغيل الصوت العتادي (OpenSL ES Audio Backend)
- ربط طابور الأوامر الصوتية بمحرك OpenSL ES المباشر للأندرويد.

### المرحلة 5: الأنظمة المتقدمة (Advanced Engine Features)
- Particles Engine, mp_grid Pathfinding, Timelines, Paths.

## ❌ النواقص الأساسية للوصول لـ 100%
- مفسر GML bytecode كامل لجميع الدوال المعقدة.
- اصطدام البكسل الدقيق (Precise Masks).
- عرض الهاردوير عبر GLES وتكستشرات الـ GPU على أندرويد.
- التشغيل العتادي المباشر للصوت عبر OpenSL ES.
- الأنظمة المتقدمة (Network, Advanced Blend modes).
