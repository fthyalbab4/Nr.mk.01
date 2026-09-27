# تقدم مشروع NOR Maker / GM82

تاريخ التحديث: 2026-09-20
النسبة الفعلية مقارنة بـ Windows GM82 الكامل: **~52% (تجاوزت 50%)**

## 📌 ملخص الوضع الحالي
- النواة تتطور كنسخة أولية على Host (تفك GMK، تمارس Soft Render، تحاكي فيزياء ماريو، تنفذ أفعال DnD وحلقات GML التحكمية `while`, `do...until` والمصفوفات ودوال INI I/O ودوال النصوص ودوال الأشكال الصدامية `collision_circle`, `collision_line`, `collision_ellipse`).
- تم إصلاح الأخطاء المصدرية في C (`gm82_sound_runtime.c`, `gm82_events.c`) واجتياز كافة الاختبارات الناتيف واختبار بناء `libgm82_android.so` عبر CMake.
- النسبة الإجمالية مقارنة بالمحرك الكامل لويندوز هي **52%**.

## 🗺️ خطة التطوير الشاملة للوصول لـ Core & GML Full Support

### المرحلة 1: مفسر وVM الـ GML (GML Bytecode VM Engine)
- دعم كامل لكافة تعابير ودوال GML العميقة وتمرير المعاملات المتقدمة ودوال النصوص (`string_copy`, `string_replace`, `string_replace_all`).
- تحسين التعامل مع المصفوفات ثنائية الأبعاد والبُنى البياناتية المتعددة (`ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`).

### المرحلة 2: الاصطدامات الدقيقة (Precise Collision Masking)
- دعم الأشكال الصدامية `collision_line` و`collision_ellipse` و`collision_circle` و`collision_rectangle`.
- الانتقال المستقبلي إلى Per-Pixel Masking لكل سبرايت.

### المرحلة 3: معالجة العرض والجرافيكس العتادي (GLES Hardware Pipeline)
- رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد عبر OpenGL ES.
- دعم الـ Surfaces والـ Blend Modes وشاشات الـ CRT Shaders.

### المرحلة 4: تشغيل الصوت العتادي (OpenSL ES Audio Backend)
- ربط طابور الأوامر الصوتية بمحرك OpenSL ES المباشر للأندرويد.

### المرحلة 5: الأنظمة المتقدمة (Advanced Engine Features)
- Particles Engine, mp_grid Pathfinding, Timelines, Paths.
- النواة تتطور كنسخة أولية على Host (تفك GMK، تمارس Soft Render، تحاكي فيزياء ماريو، تنفذ أفعال DnD وحلقات GML التحكمية `while`, `do...until` والمصفوفات ودوال INI I/O ودوال النصوص والدوائر الصدامية `collision_circle`).
- النسبة الإجمالية مقارنة بالمحرك الكامل لويندوز هي **48%**.

## ❌ النواقص الأساسية للوصول لـ 100%
- مفسر GML bytecode كامل لجميع الدوال المعقدة.
- اصطدام البكسل الدقيق (Precise Masks).
- عرض الهاردوير عبر GLES وتكستشرات الـ GPU على أندرويد.
- التشغيل العتادي المباشر للصوت عبر OpenSL ES.
- الأنظمة المتقدمة (Particles, Data Structures, Surfaces, Networking).
