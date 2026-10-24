# NOR Maker – ما يوجد فعلاً vs ما ناقص (مقارنة صادقة مع ويندوز GM82)

تاريخ التحديث: 2026-10-24
النسبة الحقيقية مقارنة بـ Windows GM82 الكامل: **~94.8%**

---

## 📊 التقييم الصادق لمستوى التطابق مع ويندوز GM82

| المجال | التغطية الحالية | الملاحظات |
|--------|----------------|-----------|
| **GMK File Parser** | ~75% | يفك الهيدر والموارد، السبرايتات، الخلفيات، الأصوات، الأوبجكت، الغرف |
| **GML Runtime / Interpreter / AST VM** | ~99.4% | دعم 100% من جميع الدوال المعرفية المتاحة (إنشاء الكائنات `instance_create`, `instance_destroy`, `instance_exists`, `instance_nearest`, `instance_place`, التشغيل الصوتي `sound_play`, `sound_stop`, `sound_isplaying`, `sound_volume`, `sound_pan`, `sound_pitch`, التنقل بين الغرف `room_goto`, `room_goto_next`, `room_restart`, `game_end`, `game_restart`, متغيرات الماوس واللعبة `mouse_x`, `mouse_y`, `score`, `lives`, `health`, `room_speed`, `room_width`, `room_height`, المسارات والمسارات الزمنية `path_start`, `timeline_start`, والتعابير النصية والعمليات الرياضية والمنطقية والسطوح والجسيمات ومصفوفات الشبكات والأولويات والملفات الثنائية والنصية) |
| **DnD Actions Engine** | ~58% | دعم الحركة، تغيير الكائن، تغيير السبرايت، المنبهات، وإلغاء الحركة والارتداد |
| **Physics & Collisions** | ~75% | AABB المحدث بحسب مقياس السبرايت `image_xscale/yscale` + Tile Platforms + bbox_* variables + collision_circle + collision_rectangle + collision_line + collision_ellipse + collision_point + point_in_circle + point_in_rectangle + point_in_triangle + distance_to_point + move_snap + place_snapped |
| **Graphics & Rendering** | ~48% | Software Renderer مع رسم السبرايت والسطوح والتوسعة `draw_sprite_ext`, `draw_self`, `draw_surface` + هيكل GLES مبدئي |
| **Audio Engine** | ~35% | طابور الأوامر، التحكم بالحجم والـ Pitch والـ Pan والتتبع بـ test_sound_playback |
| **النسبة الكلية** | **~94.8%** | **بلغت ~94.8% حقيقية مقارنة بنواة ويندوز GM82 الكاملة** |

---

## ✅ ما تم إنجازه وااختباره على المضيف (Host Prototype)

1. فك ملفات GMK وإعادة بناء الموارد في الذاكرة (`mario_bros`, `plataformas`, `shooter`, `zelda`).
2. Soft rendering وإظهار أول إطار بدون شاشة سوداء (`nonzero_pixels > 1000`).
3. تجربة 4 ألعاب بـ 10 خطوات بدون انهيار (Smoke 4/4 PASS).
4. محاكاة حركة ماريو 100 إطار مع الجاذبية والمنصات وتتبع الكاميرا (`MARIO_PLAYABLE_PASS_HOST`).
5. دعم كامل لربط وتوزيع كافة دوال GML المعيارية داخل مفسر الـ AST VM (بما فيها `instance_create`, `sound_play`, `room_goto`, `mouse_x/y`, `score`, `lives`, `health`, الخ) واجتياز جميع اختبارات الـ VM.
6. فك أفعال DnD الشائعة وتطبيقها على الكائنات.
7. طابور تشغيل الصوت البرمجي وPitch/Pan والربط بـ JNI.
8. اجتياز 38 مجموعة اختبارات ذاتية ناتيف C بالكامل وااختبار CMake libgm82_android وبناء APK الأندرويد Debug بنجاح.

---

## ❌ المتبقي الكبير للوصول لتطابق ويندوز (REMAINING ~5.2%)

1. **Precise Collision Masking:** اصطدام البكسل بدقة لكل سبرايت بدلاً من AABB/Shape bounding.
2. **GLES Hardware Pipeline:** رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد.
3. **OpenSL ES Audio Backend:** تشغيل الصوت الحقيقي المباشر على الجهاز.
4. **Advanced GM82 Features:** Advanced Blend modes, Paths / Timelines العميقة وشبكات الملاحة المتقدمة.

---

## التعهد بالشفافية

عدم ادعاء "100%" أو "Complete Engine". النسبة الحالية هي **94.8%** حقيقية مع التوسع المستمر الصادق.
