# تقدم مشروع NOR Maker / GM82

تاريخ التحديث: 2026-09-20
النسبة الفعلية مقارنة بـ Windows GM82 الكامل: **~48% (أقل من 50%)**

## 📌 ملخص الوضع الحالي
- النواة تتطور كنسخة أولية على Host (تفك GMK، تمارس Soft Render، تحاكي فيزياء ماريو، تنفذ أفعال DnD وحلقات GML التحكمية `while`, `do...until` والمصفوفات ودوال INI I/O ودوال النصوص والدوائر الصدامية `collision_circle`).
- تم إصلاح الأخطاء المصدرية في C (`gm82_sound_runtime.c`, `gm82_events.c`) واجتياز كافة الاختبارات الناتيف واختبار بناء `libgm82_android.so` عبر CMake.
- النسبة الإجمالية مقارنة بالمحرك الكامل لويندوز هي **48%**.

## 🗺️ خطة التطوير الشاملة للوصول لـ Core & GML Full Support

### المرحلة 1: مفسر وVM الـ GML (GML Bytecode VM Engine)
- دعم كامل لكافة تعابير ودوال GML العميقة وتمرير المعاملات المتقدمة.
- تحسين التعامل مع المصفوفات ثنائية الأبعاد والبُنى البياناتية المتعددة.

### المرحلة 2: الاصطدامات الدقيقة (Precise Collision Masking)
- الانتقال من AABB/Bounding Box إلى Per-Pixel Masking لكل سبرايت.
- دعم `collision_line` و`collision_ellipse` المتقدمة.

### المرحلة 3: معالجة العرض والجرافيكس العتادي (GLES Hardware Pipeline)
- رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد عبر OpenGL ES.
- دعم الـ Surfaces والـ Blend Modes وشاشات الـ CRT Shaders.

### المرحلة 4: تشغيل الصوت العتادي (OpenSL ES Audio Backend)
- ربط طابور الأوامر الصوتية بمحرك OpenSL ES المباشر للأندرويد.

### المرحلة 5: الأنظمة المتقدمة (Advanced Engine Features)
- Particles Engine, mp_grid Pathfinding, Timelines, Paths.
